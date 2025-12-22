// Stub implementations for missing vectorized STL functions
// These are needed when Firebase libraries are built with a different
// compiler version/configuration than your project

extern "C" {
    // Vectorized string search function
    void __std_search_1() {}

    // Vectorized remove function (8-byte operations)
    void __std_remove_8() {}

    // Vectorized find_last_of function
    void __std_find_last_of_trivial_pos_1() {}

    // Vectorized find_first_of function
    void __std_find_first_of_trivial_pos_1() {}

    // AVX2 wide memory comparison flag
    int _Avx2WmemEnabled = 0;
}