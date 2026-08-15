#pragma once

template <typename Fn> class Defer {
public:
    Defer(Fn func)
        : m_func { func } { };

    ~Defer() { m_func(); }

private:
    Fn m_func;
};

#define TEMP_VAR_APPEND_COUTNER(x) ___tmp_var_##x
#define TEMP_VAR_EVAL_COUNTER(x) TEMP_VAR_APPEND_COUTNER(x)
#define inl_defer(code) Defer const TEMP_VAR_EVAL_COUNTER(__COUNTER__) = [&]() { code; }
