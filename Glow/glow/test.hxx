namespace helpers
{
inline auto somefunc() { return helpers::someOtherFunc(); }

inline auto someOtherFunc() { return 1; }
} // namespace helpers

struct HelperStruct
{
    inline auto somefunc() { return someOtherFunc(); }

    inline auto someOtherFunc() -> int { return 1; }
};
