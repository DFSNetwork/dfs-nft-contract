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

struct twap
{
    uint64_t key;

    name submitter;
    string period;

    double price0_cumulative_last;
    double price1_cumulative_last;

    double price0_avg_price;
    double price1_avg_price;

    time_point_sec last_update;

    uint64_t primary_key() const { return key; }
};
typedef multi_index<"twaps"_n, twap> twaps;

struct source
{
    string key;
    double val;
};

struct iprice
{
    symbol_code symc;
    double index_price;
    time_point_sec last_update;

    uint64_t primary_key() const { return symc.raw(); }
};
typedef multi_index<name("iprices"), iprice> iprices;

struct liq2
{
    uint64_t mid;
    uint64_t token;
    asset bal0;
    asset bal1;
    time_point_sec start;

    uint64_t primary_key() const { return mid; }
};

typedef multi_index<"liqs2"_n, liq2> liqs2;

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