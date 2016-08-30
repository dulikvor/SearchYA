#pragma once

#define ENUMERATION(name) \
public: \
    name(Enumeration e):m_currentEnum(e){} \
    operator Enumeration(){return m_currentEnum;} \
\
private: \
    Enumeration m_currentEnum;
