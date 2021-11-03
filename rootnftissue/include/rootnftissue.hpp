#include <utils.hpp>

CONTRACT rootnftissue : public contract{
   public:
      using contract::contract;

      const name cname = name("root");

      ACTION claim(name nm);

      // [[eosio::on_notify("usdxusdxusdx::transfer")]]
      // void mint(name from, name to, asset quantity, string memo);

   private:
      TABLE airdrop
      {
         name        user;
         uint64_t primary_key() const { return user.value; }
      };

      typedef multi_index<name("airdrops"), airdrop> airdrops;

      TABLE property
      {
         name      key;
         string    val;
         uint64_t primary_key() const { return key.value; }
      };

      typedef multi_index<name("properties"), property> properties;

      TABLE stat
      {
         name      key;
         uint64_t    val;
         uint64_t primary_key() const { return key.value; }
      };

      typedef multi_index<name("stats"), stat> stats;

      void mint_nft(name user, string memo);

      uint64_t get_asset_id();

      uint64_t get_supply();

      void create_stats(uint64_t assetid, name key, checksum256 gene, int index);

      string get_armor_prifix(uint64_t num);
      
};