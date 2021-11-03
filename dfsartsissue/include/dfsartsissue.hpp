#include <utils.hpp>

CONTRACT dfsartsissue : public contract{
   public:
      using contract::contract;

      const name collection_name = name("dfsarts");

      ACTION issue(string title, string description, string imgurl);

   private:
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

      uint64_t get_asset_id();

      uint64_t get_supply();
};



