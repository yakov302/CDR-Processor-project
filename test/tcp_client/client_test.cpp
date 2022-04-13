#include "mu_test.h"
#include <iostream>
#include <unistd.h>
#include "cdr_struct.hpp"
#include "reader.hpp"
#include "thread.hpp" 
#include "protocol.hpp" 
#include "tcp_client.hpp" 
#include "client_manager.hpp"


BEGIN_TEST(client)

    cdr::ClientManager client ("127.0.0.1", 5555);

   char c =  '+';
   while(std::cin >> c)
   {
       switch (c)
       {
           case 'q': 

               goto exit_loop;
               break;       
       }
   }
    exit_loop:

	ASSERT_PASS();

END_TEST


BEGIN_SUITE(tread_tests)

    TEST(client)

END_SUITE
