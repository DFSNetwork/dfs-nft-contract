#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;
using namespace std;

CONTRACT dfsnftassets : public contract{
   public:
      using contract::contract;

      ACTION create(name issuer, name collection, uint64_t maximum_supply);

      ACTION issue(name to, name collection, string memo);

      ACTION transfer(name from, name to, uint64_t assetid, string memo);

      ACTION retire(uint64_t assetid, string memo);

   private:
      TABLE global_var
      {
         name        key;
         uint64_t    val;

         uint64_t primary_key() const { return key.value; }
      };

      typedef multi_index<name("globals"), global_var> globals;

      TABLE registry_st
      {
         uint64_t                id;
         uint64_t                cid; 
         name                    collection;
         name                    owner;

         uint64_t primary_key() const { return id; }
         uint64_t by_collection() const { return collection.value; }
         uint64_t by_owner() const { return owner.value; }
      };

      typedef multi_index<name("registry"), registry_st,
         indexed_by<name("collection"), const_mem_fun<registry_st, uint64_t, &registry_st::by_collection>> ,
         indexed_by<name("owner"), const_mem_fun<registry_st, uint64_t, &registry_st::by_owner>>
      > registry;

      TABLE collection_stats
      {
         name           collection;
         uint64_t       supply;
         uint64_t       max_supply;
         name           issuer;

         uint64_t primary_key() const { return collection.value; }
      };

      typedef multi_index<name("stat"), collection_stats> stats;

      TABLE asset_st {
         uint64_t                id;
         uint64_t                cid; 
         name                    collection;

         uint64_t primary_key() const { return id; }
         uint64_t by_collection() const { return collection.value; }
      };
      typedef multi_index<name("assets"), asset_st,
         indexed_by<name("collection"), const_mem_fun<asset_st, uint64_t, &asset_st::by_collection>>
      > assets;

      uint64_t get_id(name key);
};