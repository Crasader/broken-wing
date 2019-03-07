#ifndef PTMODELDECLARATION_H
#define PTMODELDECLARATION_H

#include <memory>

#define PT_MODEL_DECL(CLASS_NAME) \
    class CLASS_NAME; \
    typedef std::shared_ptr<CLASS_NAME> CLASS_NAME##Ptr

#endif // PTMODELDECLARATION_H
