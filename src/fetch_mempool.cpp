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
#include <bitcoin/blockchain/fetch_mempool.hpp>

#include <system_error>
#include <bitcoin/blockchain/blockchain.hpp>

namespace libbitcoin {
namespace chain {

typedef blockchain_fetch_mem_pool_transactions_handler handler_mem_pool;

void fetch_mem_pool_transactions(blockchain& chain,
    handler_mem_pool handle_fetch)
{
    chain.fetch_mem_pool_transaction_hashes(handle_fetch);
}

} // namespace chain
} // namespace libbitcoin
