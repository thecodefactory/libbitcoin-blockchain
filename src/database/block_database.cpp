/*
 * Copyright (c) 2011-2013 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/blockchain/database/block_database.hpp>

#include <bitcoin/satoshi_serialize.hpp>
#include <bitcoin/block.hpp>
#include <bitcoin/blockchain/database/fsizes.hpp>

namespace libbitcoin {
    namespace chain {

constexpr size_t number_buckets = 10000;
constexpr size_t header_size = htdb_slab_header_fsize(number_buckets);
constexpr size_t initial_map_file_size = header_size + min_slab_fsize;

constexpr position_type alloc_offset = header_size;

template <typename Iterator>
block_header_type deserialize(const Iterator first)
{
    block_header_type header;
    satoshi_load<Iterator, false>(first, nullptr, header);
    return header;
}

block_result::block_result(const slab_type slab)
  : slab_(slab)
{
}

block_result::operator bool() const
{
    return slab_ != nullptr;
}

block_header_type block_result::header() const
{
    BITCOIN_ASSERT(slab_);
    return deserialize(slab_);
}

size_t block_result::transactions_size() const
{
    BITCOIN_ASSERT(slab_);
    return from_little_endian<uint32_t>(slab_ + 80);
}

index_type block_result::transaction_index(size_t i) const
{
    BITCOIN_ASSERT(slab_);
    BITCOIN_ASSERT(i < transactions_size());
    const uint8_t* first = slab_ + 80 + 4 + i * 4;
    return from_little_endian<uint32_t>(first);
}

block_database::block_database(
    const std::string& map_filename, const std::string& index_filename)
  : map_file_(map_filename), header_(map_file_, 0),
    allocator_(map_file_, alloc_offset), map_(header_, allocator_),
    index_file_(index_filename),
    index_(index_file_, 0, sizeof(position_type))
{
    BITCOIN_ASSERT(map_file_.data());
    BITCOIN_ASSERT(index_file_.data());
}

void block_database::initialize_new()
{
    map_file_.resize(initial_map_file_size);
    header_.initialize_new(number_buckets);
    allocator_.initialize_new();

    index_file_.resize(min_records_fsize);
    index_.initialize_new();
}

void block_database::start()
{
    header_.start();
    allocator_.start();
    index_.start();
}

block_result block_database::get(const size_t height) const
{
    if (height >= index_.size())
        return block_result(nullptr);
    const position_type position = read_position(height);
    const slab_type slab = allocator_.get(position);
    return block_result(slab);
}

block_result block_database::get(const hash_digest& hash) const
{
    const slab_type slab = map_.get(hash);
    return block_result(slab);
}

void block_database::store(
    const size_t height, const block_header_type& header,
    const transaction_index_list& tx_indexes)
{
    // Write block data.
    const hash_digest key = hash_block_header(header);
    const size_t value_size = 80 + 4 + tx_indexes.size() * 4;
    auto write = [&header, &tx_indexes](uint8_t* data)
    {
        satoshi_save(header, data);
        auto serial = make_serializer(data + 80);
        serial.write_4_bytes(tx_indexes.size());
        for (const index_type tx_index: tx_indexes)
            serial.write_4_bytes(tx_index);
    };
    const position_type position = map_.store(key, value_size, write);
    // Write height -> position mapping.
    const index_type index = write_position(position);
    BITCOIN_ASSERT(index == height);
}

void block_database::unlink(const size_t from_height)
{
    index_.resize(from_height);
}

void block_database::sync()
{
    allocator_.sync();
    index_.sync();
}

index_type block_database::write_position(const position_type position)
{
    const index_type index = index_.allocate();
    record_type record = index_.get(index);
    auto serial = make_serializer(record);
    serial.write_8_bytes(position);
    return index;
}

position_type block_database::read_position(const index_type index) const
{
    record_type record = index_.get(index);
    return from_little_endian<position_type>(record);
}

    } // namespace chain
} // namespace libbitcoin
