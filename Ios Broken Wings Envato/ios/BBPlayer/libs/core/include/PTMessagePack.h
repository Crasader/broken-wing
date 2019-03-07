#ifndef PTMESSAGEPACK_H
#define PTMESSAGEPACK_H

#include "PTMessagePackInterface.h"

#include <cocos2d.h>
#include <msgpack.hpp>

#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {






//template <typename K, typename V, typename Hash, typename Pred, typename Alloc>
template<>
struct convert<PTMessagePack> {
    msgpack::object const& operator()(msgpack::object const& o, PTMessagePack& v) const {
        if(o.type != msgpack::type::MAP) { throw msgpack::type_error(); }
        msgpack::object_kv* p(o.via.map.ptr);
        msgpack::object_kv* const pend(o.via.map.ptr + o.via.map.size);
        PTMessagePack tmp;
        for(; p != pend; ++p) {
            PTMessagePack::Map::key_type key;
            p->key.convert(key);
            p->val.convert(tmp._map[key]);
        }
        tmp._map.swap(v._map);
        return o;
    }
};

//template <typename K, typename V, typename Hash, typename Pred, typename Alloc>
template<>
struct pack<PTMessagePack> {
    template <typename Stream>
    msgpack::packer<Stream>& operator()(msgpack::packer<Stream>& o, const PTMessagePack& v) const {
        uint32_t size = checked_get_container_size(v._map.size());
        o.pack_map(size);
        for(typename PTMessagePack::Map::const_iterator it(v._map.begin()), it_end(v._map.end());
            it != it_end; ++it) {
            o.pack(it->first);
            o.pack(it->second);
        }
        return o;
    }
};

//template <typename K, typename V, typename Hash, typename Pred, typename Alloc>
template<>
struct object_with_zone<PTMessagePack> {
    void operator()(msgpack::object::with_zone& o, const PTMessagePack& v) const {
        o.type = msgpack::type::MAP;
        if(v._map.empty()) {
            o.via.map.ptr  = MSGPACK_NULLPTR;
            o.via.map.size = 0;
        } else {
            uint32_t size = checked_get_container_size(v._map.size());
            msgpack::object_kv* p = static_cast<msgpack::object_kv*>(o.zone.allocate_align(sizeof(msgpack::object_kv)*size));
            msgpack::object_kv* const pend = p + size;
            o.via.map.ptr  = p;
            o.via.map.size = size;
            typename PTMessagePack::Map::const_iterator it(v._map.begin());
            do {
                p->key = msgpack::object(it->first, o.zone);
                p->val = msgpack::object(it->second, o.zone);
                ++p;
                ++it;
            } while(p < pend);
        }
    }
};




// PTMessagePack
/*
template<>
struct convert<PTMessagePack> {
    msgpack::object const& operator()(msgpack::object const &object, PTMessagePack &ptMap) const {
        if (object.type != msgpack::type::MAP) {
            throw msgpack::type_error();
        }

        ptMap = object.via.map.as<std::unordered_map<std::string, msgpack::object>>();

        return object;
        return convert<std::unordered_map<std::string, msgpack::object>>::operator()(object, ptMap);
    }
};

template<>
struct pack<PTMessagePack> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream> &packer, PTMessagePack const &ptMap) const {
        packer.pack_map(2);
        packer.pack(ptMap);

        return packer;
    }
};

template <>
struct object_with_zone<PTMessagePack> {
    void operator()(msgpack::object::with_zone &object, PTMessagePack const &ptMap) const {
        object.type = msgpack::type::MAP;
        object.via.map.ptr = msgpack::object(ptMap, object.zone);
    }
};

*/
// CCPoint

template<>
struct convert<cocos2d::CCPoint> {
    msgpack::object const& operator()(msgpack::object const &object, cocos2d::CCPoint &point) const {
        if (object.type != msgpack::type::ARRAY) {
            throw msgpack::type_error();
        }
        else if (object.via.array.size != 2) {
            throw msgpack::type_error();
        }

        point.x = object.via.array.ptr[0].as<float>();
        point.y = object.via.array.ptr[1].as<float>();

        return object;
    }
};

template<>
struct pack<cocos2d::CCPoint> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream> &packer, cocos2d::CCPoint const &point) const {
        packer.pack_array(2);
        packer.pack(point.x);
        packer.pack(point.y);

        return packer;
    }
};

template <>
struct object_with_zone<cocos2d::CCPoint> {
    void operator()(msgpack::object::with_zone &object, cocos2d::CCPoint const &point) const {
        object.type = msgpack::type::ARRAY;
        object.via.array.size = 2;
        object.via.array.ptr = static_cast<msgpack::object*>(object.zone.allocate_align(sizeof(msgpack::object) * object.via.array.size));
        object.via.array.ptr[0] = msgpack::object(point.x, object.zone);
        object.via.array.ptr[1] = msgpack::object(point.y, object.zone);
    }
};


// CCRect

template<>
struct convert<cocos2d::CCRect> {
    msgpack::object const& operator()(msgpack::object const &object, cocos2d::CCRect &rect) const {
        if (object.type != msgpack::type::ARRAY) {
            throw msgpack::type_error();
        }
        else if (object.via.array.size != 4) {
            throw msgpack::type_error();
        }

        rect.origin.x = object.via.array.ptr[0].as<float>();
        rect.origin.y = object.via.array.ptr[1].as<float>();
        rect.size.width = object.via.array.ptr[2].as<float>();
        rect.size.height = object.via.array.ptr[3].as<float>();

        return object;
    }
};

template<>
struct pack<cocos2d::CCRect> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream> &packer, cocos2d::CCRect const &rect) const {
        packer.pack_array(4);
        packer.pack(rect.origin.x);
        packer.pack(rect.origin.y);
        packer.pack(rect.size.width);
        packer.pack(rect.size.height);

        return packer;
    }
};

template <>
struct object_with_zone<cocos2d::CCRect> {
    void operator()(msgpack::object::with_zone &object, cocos2d::CCRect const &rect) const {
        object.type = msgpack::type::ARRAY;
        object.via.array.size = 4;
        object.via.array.ptr = static_cast<msgpack::object*>(object.zone.allocate_align(sizeof(msgpack::object) * object.via.array.size));
        object.via.array.ptr[0] = msgpack::object(rect.origin.x, object.zone);
        object.via.array.ptr[1] = msgpack::object(rect.origin.y, object.zone);
        object.via.array.ptr[2] = msgpack::object(rect.size.width, object.zone);
        object.via.array.ptr[3] = msgpack::object(rect.size.height, object.zone);
    }
};


// ccColor4F

template<>
struct convert<cocos2d::ccColor4F> {
    msgpack::object const& operator()(msgpack::object const &object, cocos2d::ccColor4F &color) const {
        if (object.type != msgpack::type::ARRAY) {
            throw msgpack::type_error();
        }
        else if (object.via.array.size != 4) {
            throw msgpack::type_error();
        }

        color.r = object.via.array.ptr[0].as<GLfloat>();
        color.g = object.via.array.ptr[1].as<GLfloat>();
        color.b = object.via.array.ptr[2].as<GLfloat>();
        color.a = object.via.array.ptr[3].as<GLfloat>();

        return object;
    }
};

template<>
struct pack<cocos2d::ccColor4F> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream> &packer, cocos2d::ccColor4F const &color) const {
        packer.pack_array(4);
        packer.pack(color.r);
        packer.pack(color.g);
        packer.pack(color.b);
        packer.pack(color.a);

        return packer;
    }
};

template <>
struct object_with_zone<cocos2d::ccColor4F> {
    void operator()(msgpack::object::with_zone &object, cocos2d::ccColor4F const &color) const {
        object.type = msgpack::type::ARRAY;
        object.via.array.size = 4;
        object.via.array.ptr = static_cast<msgpack::object*>(object.zone.allocate_align(sizeof(msgpack::object) * object.via.array.size));
        object.via.array.ptr[0] = msgpack::object(color.r, object.zone);
        object.via.array.ptr[1] = msgpack::object(color.g, object.zone);
        object.via.array.ptr[2] = msgpack::object(color.b, object.zone);
        object.via.array.ptr[3] = msgpack::object(color.a, object.zone);
    }
};


// CCSize

template<>
struct convert<cocos2d::CCSize> {
    msgpack::object const& operator()(msgpack::object const &object, cocos2d::CCSize &size) const {
        if (object.type != msgpack::type::ARRAY) {
            throw msgpack::type_error();
        }
        else if (object.via.array.size != 2) {
            throw msgpack::type_error();
        }

        size.width = object.via.array.ptr[0].as<float>();
        size.height = object.via.array.ptr[1].as<float>();

        return object;
    }
};

template<>
struct pack<cocos2d::CCSize> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream> &packer, cocos2d::CCSize const &size) const {
        packer.pack_array(2);
        packer.pack(size.width);
        packer.pack(size.height);

        return packer;
    }
};

template <>
struct object_with_zone<cocos2d::CCSize> {
    void operator()(msgpack::object::with_zone &object, cocos2d::CCSize const &size) const {
        object.type = msgpack::type::ARRAY;
        object.via.array.size = 2;
        object.via.array.ptr = static_cast<msgpack::object*>(object.zone.allocate_align(sizeof(msgpack::object) * object.via.array.size));
        object.via.array.ptr[0] = msgpack::object(size.width, object.zone);
        object.via.array.ptr[1] = msgpack::object(size.height, object.zone);
    }
};

}
}
}

#endif // PTMESSAGEPACK_H
