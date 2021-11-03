#include <rootnftissue.hpp>

void rootnftissue::claim(name user)
{
   require_auth(user);

   airdrops _airdrops(get_self(), get_self().value);
   auto itr = _airdrops.find(user.value);
   check(itr == _airdrops.end(), "already claimed");

   liqs2 _liqs(name("defislogsone"), user.value);
   auto liq_itr = _liqs.begin();
   bool can_claim = false;
   // DFS中的做市商，不分做市金额大小、做市时间7天以上即可claim。
   while (liq_itr != _liqs.end())
   {
      auto time_elapsed = current_time_point().sec_since_epoch() - liq_itr->start.sec_since_epoch();
      uint32_t day_3 = 86400 * 3;
      if (time_elapsed >= day_3)
         can_claim = true;
      liq_itr++;
   }

   check(can_claim, "unable to claim");
   mint_nft(user, string("free claim ROOT NFT"));
   _airdrops.emplace(get_self(), [&](auto &s) {
      s.user = user;
   });
}

void rootnftissue::mint_nft(name user, string memo)
{
   uint64_t assetid = get_asset_id();
   string mixed = to_string(assetid);
   checksum256 gene = sha256(mixed.c_str(), sizeof(mixed.c_str()));

   // issue nft
   action(permission_level{ get_self(), name("active") }, name("dfsnftassets"), name("issue"),
      make_tuple(get_self(), cname, memo))
      .send();

   // transfer nft to user
   action{ permission_level{get_self(), name("active")}, name("dfsnftassets"), name("transfer"),
      make_tuple(get_self(), user, assetid, memo) }.send();

   // 写入元数据
   // 写入属性元数据 : Numerical traits that just show as numbers

   create_stats(assetid, name("strength"), gene, 0);  // 体力
   create_stats(assetid, name("magic"), gene, 1);     // 法力
   create_stats(assetid, name("patt"), gene, 2);      // 物理攻击力
   create_stats(assetid, name("matt"), gene, 3);      // 魔法攻击力
   create_stats(assetid, name("pdef"), gene, 4);      // 物理防御力
   create_stats(assetid, name("mdef"), gene, 5);      // 魔法防御力
   create_stats(assetid, name("attspeed"), gene, 6);  // 攻速
   create_stats(assetid, name("mvspeed"), gene, 7);   // 移动速度
   create_stats(assetid, name("crit"), gene, 8);      // 暴击
   create_stats(assetid, name("lucky"), gene, 9);     // 幸运

   // // 生日属性
   // stats _stats(get_self(), assetid);
   // _stats.emplace(get_self(), [&](auto &s) {
   //    s.key = name("birthday");
   //    s.val = current_time_point().sec_since_epoch();
   // });

   // 写入标签元数据: Textual traits 
   properties _properties(get_self(), assetid);

   string weapons[] = { "屠龙刀", "倚天剑", "杀猪刀", "射雕弓", "魔法书", "打狗棒", "诛仙剑", "镰刀", "搅屎棍", "牙签", "碧水剑", "柳叶刀", "开山斧", "双节棍", "AK47" };
   uint64_t weapons_value = gene.extract_as_byte_array()[10];
   int weapons_length = std::end(weapons) - std::begin(weapons);
   string weapon = weapons[weapons_value % weapons_length];
   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("weapon");
      s.val = weapon;
   });

   string weapons_prefix[] = { "蕴含着BTC之力的", "蕴含着ETH之力的", "蕴含着EOS之力的", "蕴含着TRX之力的", "蕴含着BNB之力的", "蕴含着HT之力的", "蕴含着OKB之力的", "蕴含着BCH之力的", "蕴含着XMR之力的", "蕴含着DFS之力的" };
   uint64_t weapons_prefix_value = gene.extract_as_byte_array()[11];
   int weapons_prefix_length = std::end(weapons_prefix) - std::begin(weapons_prefix);
   string weapons_prefix_pick = weapons_prefix[weapons_prefix_value % weapons_prefix_length];
   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("weapon.type");
      s.val = weapons_prefix_pick;
   });

   // 上衣
   uint64_t clothes_prefix_value = gene.extract_as_byte_array()[12];
   string clothes_prefix_pick = get_armor_prifix(clothes_prefix_value);
   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("clothes.pre");
      s.val = clothes_prefix_pick;
   });

   string clothes[] = { "中本聪布甲", "防爆仓背心", "防割皮衣", "开单必胜衣", "回本西服", "百倍衣", "套牢者上衣", "羊毛上衣", "链甲", "防弹背心" };
   uint64_t clothes_value = gene.extract_as_byte_array()[13];
   int clothes_length = std::end(clothes) - std::begin(clothes);
   string clothes_pick = clothes[clothes_value % clothes_length];
   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("clothes");
      s.val = clothes_pick;
   });

   // 下衣
   uint64_t pants_prefix_value = gene.extract_as_byte_array()[14];
   string pants_prefix_pick = get_armor_prifix(pants_prefix_value);
   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("pant.pre");
      s.val = pants_prefix_pick;
   });

   string pants[] = { "中本聪短裤", "防爆仓短裤", "防割秋裤", "开单必胜裤", "回本西裤", "百倍裤", "套牢者短裤", "羊毛短裤", "锁链下衣", "防弹下衣" };
   uint64_t pants_value = gene.extract_as_byte_array()[15];
   int pants_length = std::end(pants) - std::begin(pants);
   string pants_pick = pants[pants_value % pants_length];
   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("pant");
      s.val = pants_pick;
   });

   // 手套
   uint64_t gloves_prefix_value = gene.extract_as_byte_array()[16];
   string gloves_prefix_pick = get_armor_prifix(gloves_prefix_value);
   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("gloves.pre");
      s.val = gloves_prefix_pick;
   });

   string gloves[] = { "中本聪手套", "防爆仓皮手套", "防割手套", "开单必胜套", "回本手套", "百倍手套", "套牢者手套", "羊毛手套", "锁链手套", "防弹手套" };
   uint64_t gloves_value = gene.extract_as_byte_array()[17];
   int gloves_length = std::end(gloves) - std::begin(gloves);
   string gloves_pick = gloves[gloves_value % gloves_length];
   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("gloves");
      s.val = gloves_pick;
   });

   // 鞋子
   uint64_t shoes_prefix_value = gene.extract_as_byte_array()[18];
   string shoes_prefix_pick = get_armor_prifix(shoes_prefix_value);
   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("shoes.pre");
      s.val = shoes_prefix_pick;
   });

   string shoes[] = { "中本聪拖鞋", "防爆仓拖鞋", "防割皮鞋", "开单必胜鞋", "回本皮鞋", "百倍鞋", "套牢者皮鞋", "羊毛鞋", "锁链鞋", "防弹鞋" };
   uint64_t shoes_value = gene.extract_as_byte_array()[19];
   int shoes_length = std::end(shoes) - std::begin(shoes);
   string shoes_pick = shoes[shoes_value % shoes_length];
   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("shoes");
      s.val = shoes_pick;
   });

   // 帽子
   uint64_t hats_prefix_value = gene.extract_as_byte_array()[20];
   string hats_prefix_pick = get_armor_prifix(hats_prefix_value);
   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("hats.pre");
      s.val = hats_prefix_pick;
   });

   string hats[] = { "中本聪草帽", "防爆仓巫师帽", "防割棉帽", "开单必胜帽", "回本帽", "百倍王冠", "套牢者鸭舌帽", "羊毛帽", "金属帽", "防弹头盔" };
   uint64_t hats_value = gene.extract_as_byte_array()[21];
   int hats_length = std::end(hats) - std::begin(hats);
   string hats_pick = hats[hats_value % hats_length];
   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("hats");
      s.val = hats_pick;
   });

   //戒指
   string rings[] = { "魔戒", "圣光之戒", "武神之戒", "死亡之戒", "神圣天使之戒", "军团长之戒", "防暴仓戒指", "回本戒指", "百倍戒指", "套牢戒指" };
   uint64_t rings_value = gene.extract_as_byte_array()[22];
   int rings_length = std::end(rings) - std::begin(rings);
   string rings_pick = rings[rings_value % rings_length];
   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("rings");
      s.val = rings_pick;
   });

   // 项链
   string necklaces[] = { "蓝宝石项链", "红宝石项链", "绿宝石项链", "金项链", "钻石项链" , "防暴仓项链", "回本项链", "百倍项链", "套牢项链" };
   uint64_t necklaces_value = gene.extract_as_byte_array()[23];
   int necklaces_length = std::end(necklaces) - std::begin(necklaces);
   string necklaces_pick = necklaces[necklaces_value % necklaces_length];
   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("necklaces");
      s.val = necklaces_pick;
   });

   // 称号
   string titles[] = { "名扬四海的", "平平无奇的", "闻风丧胆的", "人见人爱的", "人人喊打的", "征服过世界的", "一刀999的", "新手村的王者", "客服专家", "资深老司机", "重仓EOS的", "重仓BTC的", "炒币天才", "模范韭菜", "资深羊毛党", "爆仓之王" };
   uint64_t titles_value = gene.extract_as_byte_array()[24];
   int titles_length = std::end(titles) - std::begin(titles);
   string titles_pick = titles[titles_value % titles_length];
   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("titles");
      s.val = titles_pick;
   });

   // 坐骑
   string mounts[] = { "凤凰自行车", "美团电动车", "保时洁三轮车", "敞篷五菱宏光", "漏水的奔驰大G", "耀眼的帕拉梅拉", "闪光的玛莎拉蒂", "带电的劳斯莱斯" };
   uint64_t mounts_value = gene.extract_as_byte_array()[25];
   int mounts_length = std::end(mounts) - std::begin(mounts);
   string mounts_pick = mounts[mounts_value % mounts_length];
   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("mounts");
      s.val = mounts_pick;
   });

   // 武器的稀有程度
   uint64_t weapon_prefix2_value = gene.extract_as_byte_array()[26];
   string weapon_prefix2_pick = get_armor_prifix(weapon_prefix2_value);
   _properties.emplace(get_self(), [&](auto &s) {
      s.key = name("weapon.pre");
      s.val = weapon_prefix2_pick;
   });
}

string rootnftissue::get_armor_prifix(uint64_t num)
{
   // 45% 25% 15% 10% 5%
   string armor_prefix[] = { "普通的", "稀有的", "无暇的", "完美的", "传说的" };
   if (num < 115)
      return armor_prefix[0];
   else if (num < 178)
      return armor_prefix[1];
   else if (num < 216)
      return armor_prefix[2];
   else if (num < 242)
      return armor_prefix[3];
   else
      return armor_prefix[4];
}

void rootnftissue::create_stats(uint64_t assetid, name key, checksum256 gene, int index)
{
   stats _stats(get_self(), assetid);
   uint64_t value = gene.extract_as_byte_array()[index];
   _stats.emplace(get_self(), [&](auto &s) {
      s.key = key;
      s.val = value;
   });
}

// void rootnftissue::mint(name from, name to, asset quantity, string memo)
// {
//    if (from == _self || to != _self)
//       return;
//    uint64_t supply = get_supply();
//    asset mint_fee = asset((supply + 1) * 10000ll, symbol("USDC", 4));
//    // check(quantity == mint_fee, string("invalid quantity: require " + mint_fee.to_string() + " to mint ROOT NFT").c_str());
//    mint_nft(from, string("mint ROOT NFT"));
// }

uint64_t rootnftissue::get_supply()
{
   c_stats_index _stats(name("dfsnftassets"), name("dfsnftassets").value);
   auto itr = _stats.find(cname.value);
   uint64_t supply = 0;
   if (itr != _stats.end())
   {
      supply = itr->supply;
   }
   return supply;
}

uint64_t rootnftissue::get_asset_id()
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