/**
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
#ifndef LIBBITCOIN_BLOCKCHAIN_MEMPOOL_HPP
#define LIBBITCOIN_BLOCKCHAIN_MEMPOOL_HPP

#include <cstdint>
#include <system_error>
#include <bitcoin/bitcoin.hpp>
#include <bitcoin/blockchain/define.hpp>
#include <bitcoin/blockchain/blockchain.hpp>

namespace libbitcoin {
namespace chain {

typedef std::function<void (const std::error_code&, const hash_list&)>
    blockchain_fetch_mem_pool_transactions_handler;

/**
 * Fetches list of transaction hashes in the mempool.
 *
 * @param[in]   chain           Blockchain service
 * @param[in]   handle_fetch    Completion handler for fetch operation.
 * @code
 *  void handle_fetch(
 *      const std::error_code& ec,  // Status of operation
 *      const hash_list& hashes     // List of hashes
 *  );
 * @endcode
 */
BCB_API void fetch_mem_pool_transaction_hashes(blockchain& chain,
    blockchain_fetch_mem_pool_transactions_handler handle_fetch);

} // namespace chain
} // namespace libbitcoin

#endif

