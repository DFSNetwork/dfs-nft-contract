#include <dfsartsissue.hpp>

ACTION dfsartsissue::issue(string title, string description, string imgurl)
{
   require_auth(get_self());

   uint64_t assetid = get_asset_id();

   properties _properties(get_self(), assetid);

   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("title");
      s.val = title;
   });

   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("description");
      s.val = description;
   });

   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("imgurl");
      s.val = imgurl;
   });

   // issue nft
   action(permission_level{ get_self(), name("active") }, name("dfsnftassets"), name("issue"),
      make_tuple(get_self(), collection_name, string("issue dfsarts NFT")))
      .send();
}

uint64_t dfsartsissue::get_supply()
{
   c_stats_index _stats(name("dfsnftassets"), name("dfsnftassets").value);
   auto itr = _stats.find(collection_name.value);
   uint64_t supply = 0;
   if (itr != _stats.end())
   {
      supply = itr->supply;
   }
   return supply;
}

uint64_t dfsartsissue::get_asset_id()
{
   globals _globals(name("dfsnftassets"), name("dfsnftassets").value);
   auto itr = _globals.find(name("assetid").value);
   uint64_t id = 1;
   if (itr != _globals.end())
   {
      id = itr->val + 1;
   }
   return id;
}