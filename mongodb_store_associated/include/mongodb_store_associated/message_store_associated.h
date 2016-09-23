#ifndef MESSAGE_STORE_ASSOCIATED_H
#define MESSAGE_STORE_ASSOCIATED_H

#include <mongodb_store/message_store.h>
#include <cereal/archives/json.hpp>

namespace cereal
{
//! Loading for std::map<std::string, std::string> for text based archives
template <class Archive, class C, class A>
typename std::enable_if<traits::is_text_archive<Archive>::value, void>::type
load(Archive& ar, std::map<std::string, std::string, C, A>& map)
{
    map.clear();

    auto hint = map.begin();
    while (true) {
        const auto namePtr = ar.getNodeName();
        if(!namePtr) {
            break;
        }
        std::string key = namePtr;
        std::string value; ar( value );
        hint = map.emplace_hint( hint, std::move( key ), std::move( value ) );
    }
}
} // namespace cereal

template <typename Msg1, typename Msg2>
Msg1 get_associated_mongodb_field(mongodb_store::MessageStoreProxy& message_store, const Msg2& message, const std::string& field)
{
    std::stringstream associated_mongodb_fields_map;
    associated_mongodb_fields_map << "{\"value0\":  " << message.associated_mongodb_fields_map << "}";
    std::map<std::string, std::string> associated_mongodb_fields;
    {
        cereal::JSONInputArchive archive_i(associated_mongodb_fields_map);
        archive_i(associated_mongodb_fields);
    }
    std::string mongodb_id = associated_mongodb_fields.at(field);
    std::pair<boost::shared_ptr<Msg1>, mongo::BSONObj> query = message_store.queryID<Msg1>(mongodb_id);
    return *query.first;
}

#endif // MESSAGE_STORE_ASSOCIATED_H
