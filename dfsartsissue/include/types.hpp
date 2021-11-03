#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/singleton.hpp>
#include <eosio/crypto.hpp>
#include <eosio/transaction.hpp>
#include <math.h>

using namespace eosio;
using namespace std;

struct currency_stats
{
    asset supply;
    asset max_supply;
    name issuer;

    uint64_t primary_key() const { return supply.symbol.code().raw(); }
};

struct account
{
    asset balance;

    uint64_t primary_key() const { return balance.symbol.code().raw(); }
};

typedef eosio::multi_index<"accounts"_n, account> accounts;
typedef eosio::multi_index<"stat"_n, currency_stats> stats;

struct global_var
{
    name        key;
    uint64_t    val;

    uint64_t primary_key() const { return key.value; }
};

typedef multi_index<name("globals"), global_var> globals;

struct collection_stats
{
    name           collection;
    uint64_t       supply;
    uint64_t       max_supply;
    name           issuer;

    uint64_t primary_key() const { return collection.value; }
};

typedef multi_index<name("stat"), collection_stats> c_stats_index;