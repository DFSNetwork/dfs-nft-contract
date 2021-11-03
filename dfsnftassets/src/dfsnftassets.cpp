#include <dfsnftassets.hpp>

void dfsnftassets::create(name issuer, name collection, uint64_t maximum_supply)
{
   require_auth(get_self());

   check(maximum_supply > 0, "max-supply must be positive");

   stats statstable(get_self(), get_self().value);
   auto existing = statstable.find(collection.value);
   check(existing == statstable.end(), "collection already exists");

   statstable.emplace(get_self(), [&](auto &s) {
      s.collection = collection;
      s.max_supply = maximum_supply;
      s.issuer = issuer;
   });
}

void dfsnftassets::issue(name to, name collection, string memo)
{
   check(memo.size() <= 256, "memo has more than 256 bytes");

   stats statstable(get_self(), get_self().value);
   auto existing = statstable.find(collection.value);
   check(existing != statstable.end(), "collection does not exist, create collection before issue");
   const auto &st = *existing;
   check(to == st.issuer, "nft can only be issued to issuer account");

   require_auth(st.issuer);

   check(1 + st.supply <= st.max_supply, "supply exceeds max supply");
   statstable.modify(st, same_payer, [&](auto &s) {
      s.supply += 1;
   });

   uint64_t asset_id = get_id(name("assetid"));
   uint64_t collection_id = get_id(collection);

   assets to_acnts(get_self(), st.issuer.value);
   to_acnts.emplace(st.issuer, [&](auto &a) {
      a.id = asset_id;
      a.cid = collection_id;
      a.collection = collection;
   });

   registry _registry(get_self(), get_self().value);
   _registry.emplace(st.issuer, [&](auto &a) {
      a.id = asset_id;
      a.cid = collection_id;
      a.collection = collection;
      a.owner = st.issuer;
   });
}

void dfsnftassets::transfer(name from, name to, uint64_t assetid, string memo)
{
   check(from != to, "cannot transfer to yourself");
   require_auth(from);
   check(is_account(to), "to account does not exist");
   check(memo.size() <= 256, "memo has more than 256 bytes");

   require_recipient(from);
   require_recipient(to);

   assets assets_f(_self, from.value);
   assets assets_t(_self, to.value);

   const auto itr = assets_f.require_find(assetid, string("Asset id: " + to_string(assetid) + " cannot be found").c_str());
   auto payer = has_auth(to) ? to : from;

   assets_t.emplace(payer, [&](auto &s) {
      s.id = itr->id;
      s.cid = itr->cid;
      s.collection = itr->collection;
   });

   registry _registry(get_self(), get_self().value);
   const auto reg_itr = _registry.require_find(assetid, string("Asset id: " + to_string(assetid) + " cannot be found").c_str());
   _registry.modify(reg_itr, payer, [&](auto &s) {
      s.owner = to;
   });

   assets_f.erase(itr);
}

void dfsnftassets::retire(uint64_t assetid, string memo)
{
   check(memo.size() <= 256, "memo has more than 256 bytes");
   registry _registry(get_self(), get_self().value);
   auto reg_itr = _registry.require_find(assetid, "asset not found");
   
   stats _stats(get_self(), get_self().value);
   auto stat_itr = _stats.require_find(reg_itr->collection.value, "collection not found");
   require_auth(stat_itr->issuer);
   
   assets _assets(get_self(), stat_itr->issuer.value);
   auto asset_itr = _assets.require_find(assetid, "asset not found");
   
   _stats.modify(stat_itr, same_payer, [&](auto &s) {
      s.supply -= 1;
   });
   _assets.erase(asset_itr);
   _registry.erase(reg_itr);
}

uint64_t dfsnftassets::get_id(name key)
{
   globals _globals(get_self(), get_self().value);
   auto itr = _globals.find(key.value);
   uint64_t id = 1;
   if (itr != _globals.end())
   {
      id = itr->val + 1;
      _globals.modify(itr, same_payer, [&](auto &s) {
         s.key = key;
         s.val = id;
      });
   }
   else
   {
      _globals.emplace(get_self(), [&](auto &s) {
         s.key = key;
         s.val = id;
      });
   }
   return id;
}

