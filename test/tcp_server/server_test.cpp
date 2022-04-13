#include "mu_test.h"
#include <iostream>
#include <unistd.h>
#include "aggregator.hpp"
#include "aggregate_data.hpp"
#include "cdr_struct.hpp"
#include "data_base.hpp"
#include "processor.hpp"
#include "biling_data.hpp"
#include "thread.hpp"
#include "protocol.hpp"
#include "tcp_server.hpp"
#include "tcp_server.hpp"
#include "blocking_queue.hpp"
#include "server_manager.hpp"
#include "http_server.hpp"

BEGIN_TEST(server)

    cdr::ServerManager s("0.0.0.0", 5555);

    char c = '+';
    while (std::cin >> c)
    {
        switch (c)
        {
        case 'p':
            s.print();
            break;

        case 'q':
            s.stop();
            goto exit_loop;
            break;
        }
    }
    exit_loop : 

    ASSERT_PASS();

END_TEST

BEGIN_SUITE(tread_tests)

    TEST(server)

END_SUITE
