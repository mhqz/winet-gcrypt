cmake_minimum_required (VERSION 3.13)

link_libraries(
    Boost::iostreams
    Boost::system
    ouinet::base
)

add_executable(test_file_io
        "test_file_io.cpp"
        "../src/util/file_io.cpp"
        "util/base_fixture.hpp")

add_executable(test_persistent_lru_cache
    "test_persistent_lru_cache.cpp"
    "../src/util/file_io.cpp"
    "../src/util/persistent_lru_cache.cpp"
    "../src/util/hash.cpp"
    "util/base_fixture.hpp")
target_link_libraries(test_persistent_lru_cache lib::gcrypt)

add_executable(test-http-store
    "test_http_store.cpp"
    "../src/cache/http_sign.cpp"
    "../src/cache/http_store.cpp"
    "../src/cache/hash_list.cpp"
    "../src/http_util.cpp"
    "../src/logger.cpp"
    "../src/response_part.cpp"
    "../src/util.cpp"
    "../src/util/atomic_dir.cpp"
    "../src/util/atomic_file.cpp"
    "../src/util/crypto.cpp"
    "../src/util/file_io.cpp"
   "../src/util/hash.cpp"
    "../src/util/temp_dir.cpp"
    "../src/util/temp_file.cpp"
)
target_link_libraries(test-http-store lib::gcrypt lib::uri)
