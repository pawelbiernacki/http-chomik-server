#ifndef HTTP_SERVER_CHOMIK_H
#define HTTP_SERVER_CHOMIK_H

#include <chomik.h>

namespace http_chomik_server
{

    class machine: public chomik::machine
    {
    protected:
        virtual void http_server_loop(int my_port);

    public:
        machine() = default;
        virtual ~machine() = default;

        virtual void create_predefined_variables() override;

        virtual bool get_is_user_defined_executable(const chomik::signature & s) const override;

        virtual void execute_user_defined_executable(const chomik::signature & s) override;
    };
}

#endif
