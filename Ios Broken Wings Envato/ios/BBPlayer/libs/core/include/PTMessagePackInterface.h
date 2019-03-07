#ifndef PTMESSAGEPACKINTERFACE_H
#define PTMESSAGEPACKINTERFACE_H

#if defined(_MSC_VER)
#include <msgpack/object_decl.hpp>
#else
#include <msgpack.hpp>
#endif

#include <cocos2d.h>
#include <unordered_map>

#define mpObj(object_, zone_) (std::move(msgpack::object(object_, zone_)))

inline bool operator==(const cocos2d::CCPoint &one, const cocos2d::CCPoint &two) {
    return one.equals(two);
}

inline bool operator==(const cocos2d::CCSize &one, const cocos2d::CCSize &two) {
    return one.equals(two);
}

inline bool operator==(const cocos2d::CCRect &one, const cocos2d::CCRect &two) {
    return one.equals(two);
}

inline bool operator==(const cocos2d::_ccColor4F &one, const cocos2d::_ccColor4F &two) {
    return one.a == two.a && one.r == two.r && one.g == two.g && one.b == two.b;
}

namespace std { using ::operator==; }

using KeyIndex = uint32_t;

class PTMessagePack {
public:
    PTMessagePack();
    PTMessagePack(msgpack::zone &zone);

    void setZone(msgpack::zone &zone);
    msgpack::zone &zone();
    bool empty();

    static void beginPack();
    static void endPack(PTMessagePack &keysMapOut);
    static void beginUnpack(const PTMessagePack &keysMapObjects);

    static KeyIndex getIndex(const std::string &key);
    static const std::string &getKey(KeyIndex index);

    template <typename Type>
    bool unpack(const std::string &field, Type &variable) const {
        KeyIndex index = getIndex(field);
        if (index != static_cast<KeyIndex>(-1)) {
            return _map.unpack(index, variable);
        }
        return false;
    }

    template <typename Type>
    void pack(const std::string &field, const Type &variable) {
        if (!_zone) {
            throw std::logic_error("Zone not set");
        }

        _map.pack(addKey(field), variable, *_zone);
    }

//private:
    class Map : public std::unordered_map<KeyIndex, msgpack::object> {
    public:
        using Base = std::unordered_map<KeyIndex, msgpack::object>;
        using Base::unordered_map;


        template <typename Type>
        bool unpack(KeyIndex field, Type &variable) const {
            auto it = find(field);
            if (it != end()) {
                variable = it->second.as<Type>();
                return true;
            }
            return false;
        }

        template <typename Type>
        void pack(KeyIndex field, const Type &variable, msgpack::zone &zone) {
            emplace(field, mpObj(variable, zone));
        }

    private:
        friend class PTMessagePack;

        using Base::emplace;
        using Base::at;
        using Base::find;
        using Base::count;
    };

    Map _map;
    msgpack::zone *_zone;

private:
    static std::unordered_map<std::string, KeyIndex> _keysMap;
    static std::unordered_map<KeyIndex, std::string> _keysIndexes;
    static int _extIndexesCount;

    static KeyIndex addKey(const std::string &key);
};

class PTMessagePackInterface {
public:
    virtual void pack(PTMessagePack &out) const {}
    virtual void unpack(const PTMessagePack &objects) {}
    virtual void packConnections(PTMessagePack &out) const {}
    virtual void unpackConnections(const PTMessagePack &objects) {}
};

#endif // PTMESSAGEPACKINTERFACE_H
