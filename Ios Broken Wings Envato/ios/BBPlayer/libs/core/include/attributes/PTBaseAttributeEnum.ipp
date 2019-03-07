#ifndef PTBASEATTRIBUTEENUM_IPP
#define PTBASEATTRIBUTEENUM_IPP

#include "PTBaseAttributeEnum.h"

#include <cocos2d.h>
#include "PTMessagePack.h"

template <typename EnumType, typename _Trait>
PTBaseAttributeEnum<EnumType, _Trait>::PTBaseAttributeEnum(const std::string &name, PTModel *model, ValueType valueType)
    :PTAttribute(name, model, valueType) {

    _value = static_cast<EnumType>(0);
}

template <typename EnumType, typename _Trait>
PTBaseAttributeEnum<EnumType, _Trait>::PTBaseAttributeEnum(const std::string &name, const PTBaseAttributeEnum<EnumType> &other)
    :PTBaseAttributeEnum<EnumType, _Trait>(name, other.model(), other) {

}

template <typename EnumType, typename _Trait>
PTBaseAttributeEnum<EnumType, _Trait>::PTBaseAttributeEnum(PTModel *model, const PTBaseAttributeEnum<EnumType> &other)
    :PTBaseAttributeEnum<EnumType, _Trait>(other.name(), model, other) {

}

template <typename EnumType, typename _Trait>
PTBaseAttributeEnum<EnumType, _Trait>::PTBaseAttributeEnum(const std::string &name, PTModel *model, const PTBaseAttributeEnum<EnumType> &other)
    :PTAttribute(name, model, other) {

    setValue(other.value(), true);
}

template <typename EnumType, typename _Trait>
PTBaseAttributeEnum<EnumType>& PTBaseAttributeEnum<EnumType, _Trait>::operator =(const PTBaseAttributeEnum<EnumType> &other) {
    PTAttribute::operator =(other);

    setValue(other.value());

    return *this;
}

template <typename EnumType, typename _Trait>
void PTBaseAttributeEnum<EnumType, _Trait>::initWithDictionary(cocos2d::CCDictionary *dictionary) {
    PTAttribute::initWithDictionary(dictionary);

    const cocos2d::CCString *value = dictionary->valueForKey("value");
    if (value && value->length() > 0) {
        setValue(static_cast<EnumType>(value->intValue()), true);
    }
}

template <typename EnumType, typename _Trait>
cocos2d::CCDictionary* PTBaseAttributeEnum<EnumType, _Trait>::getDictionary() const {
    cocos2d::CCDictionary *dictionary = PTAttribute::getDictionary();

    if (dictionary) {
        dictionary->setObject(cocos2d::CCString::createWithFormat("%d", value()) , "value");
    }

    return dictionary;
}

template <typename EnumType, typename _Trait>
EnumType PTBaseAttributeEnum<EnumType, _Trait>::value() const noexcept {
    return _value;
}

template <typename EnumType, typename _Trait>
void PTBaseAttributeEnum<EnumType, _Trait>::setValue(EnumType value, bool silent) {
    _value = value;

    if (!silent) {
        changed();
    }
}

template <typename EnumType, typename _Trait>
void PTBaseAttributeEnum<EnumType, _Trait>::unpack(const PTMessagePack &objects) {
    int value = 0;
    objects.unpack("enum", value);
    setValue(static_cast<EnumType>(value), true);
}

template <typename EnumType, typename _Trait>
void PTBaseAttributeEnum<EnumType, _Trait>::pack(PTMessagePack &out) const {
    out.pack("enum", static_cast<int>(value()));
}

#endif // PTBASEATTRIBUTEENUM_IPP
