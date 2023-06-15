#include "util/signal.h"

#include <boost/asio/spawn.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/write_at.hpp>
#include "../lib/asio/windows/random_access_handle.hpp"
#include <boost/filesystem.hpp>

namespace asio = boost::asio;
namespace errc = boost::system::errc;
namespace fs = boost::filesystem;
namespace sys = boost::system;

using aio_file_t = random_access_handle_extended;
using native_handle_t = HANDLE;

using Cancel = ouinet::Signal<void()>;

int main() {
    asio::io_context ctx;
    sys::error_code ec_write;
    Cancel cancel;

    //TODO: Convert into unit tests
    asio::spawn(ctx, [&](asio::yield_context yield){
        auto p = fs::path("c:\\temp\\test.txt");
        native_handle_t file = ::CreateFile(
                                    p.string().c_str(),
                                    GENERIC_READ | GENERIC_WRITE,       // DesiredAccess
                                    FILE_SHARE_READ | FILE_SHARE_WRITE, // ShareMode
                                    NULL,                               // SecurityAttributes
                                    OPEN_ALWAYS,                        // CreationDisposition
                                    FILE_FLAG_OVERLAPPED | FILE_FLAG_SEQUENTIAL_SCAN, // FlagsAndAttributes
                                    NULL);                              // TemplateFile

        aio_file_t aio_file = aio_file_t(ctx);
        if (file != INVALID_HANDLE_VALUE) {
            aio_file.assign(file);

            asio::async_write_at(
                    aio_file,
                    0,
                    boost::asio::const_buffer("test", 4),
                    [&ec_write](const boost::system::error_code& ec,
                                std::size_t bytes_transferred){ec_write = std::move(ec);});

            asio::steady_timer timer{ctx};
            timer.expires_from_now(std::chrono::seconds(2));
            timer.async_wait(yield);
        }
    });

    ctx.run();

    if(ec_write){
        return 1;
    }

    return 0;
}
