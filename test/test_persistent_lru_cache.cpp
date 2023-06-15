#define BOOST_TEST_MODULE Tests for persistent_lru_cache module
#include <boost/test/included/unit_test.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include "util/signal.h"
#include "util/file_io.h"
#include "util/persistent_lru_cache.h"
#include "../test/util/base_fixture.hpp"

namespace asio = boost::asio;
namespace sys = boost::system;
namespace ut = boost::unit_test;
namespace file_io = ouinet::util::file_io;

using Cancel = ouinet::Signal<void()>;

struct fixture_persistent_lru_cache:fixture_base
{
    asio::io_context ctx;
    sys::error_code ec;
    size_t default_timer = 2;
    Cancel cancel;

};

BOOST_FIXTURE_TEST_SUITE(suite_persistent_lru_cache, fixture_persistent_lru_cache);

BOOST_AUTO_TEST_CASE(test_is_regular_file)
{
    temp_file temp_file{test_id};
        asio::spawn(ctx, [&](asio::yield_context yield){
            asio::steady_timer timer{ctx};
            timer.expires_from_now(std::chrono::seconds(default_timer));
            timer.async_wait(yield);
            auto aio_file = file_io::open_or_create(
                    ctx.get_executor(),
                    temp_file.get_name(),
                    ec);
    });
    ctx.run();
    BOOST_TEST(boost::filesystem::exists(temp_file.get_name()));
    BOOST_TEST(boost::filesystem::is_regular(temp_file.get_name(), ec));
}

BOOST_AUTO_TEST_CASE(test_is_directory)
{
    temp_file temp_file{test_id};

    asio::spawn(ctx, [&](asio::yield_context yield) {
        bool success = file_io::check_or_create_directory(
                temp_file.get_name(), ec);
        BOOST_CHECK(success);
        asio::steady_timer timer{ctx};
        timer.expires_from_now(std::chrono::seconds(default_timer));
        timer.async_wait(yield);
    });
    ctx.run();
    BOOST_REQUIRE(boost::filesystem::exists(temp_file.get_name()));
    BOOST_CHECK(boost::filesystem::is_directory(temp_file.get_name()));
}

BOOST_AUTO_TEST_SUITE_END();
