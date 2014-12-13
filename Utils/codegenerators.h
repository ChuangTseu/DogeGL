#ifndef CODEGENERATORS_H
#define CODEGENERATORS_H

#define GENERATE_FiveDefaultsRule_NoDefaultDestructor(ClassName)    \
    ClassName(const ClassName& other) = default;                    \
    ClassName(ClassName&& other) = default;                         \
    ClassName& operator=(const ClassName& other) = default;         \
    ClassName& operator=(ClassName&& other) = default;

#define GENERATE_FiveDefaultsRule(ClassName)                        \
    GENERATE_FiveDefaultsRule_NoDefaultDestructor(ClassName)        \
    ~ClassName() = default;

#endif // CODEGENERATORS_H
