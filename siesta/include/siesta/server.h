#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "common.h"

namespace siesta
{
    namespace server
    {
        class Route
        {
        public:
            virtual ~Route() = default;
        };

        class RouteHolder
        {
            std::vector<std::unique_ptr<Route>> routes_;

        public:
            RouteHolder& operator+=(std::unique_ptr<Route> route);
        };

        class Request
        {
        public:
            virtual ~Request()                        = default;
            virtual const std::string& getUri() const = 0;
            virtual const Method getMethod() const    = 0;
            virtual const std::map<std::string, std::string>& getUriParameters()
                const = 0;
            virtual const std::map<std::string, std::string>& getQueries()
                const                                                   = 0;
            virtual std::string getHeader(const std::string& key) const = 0;
            virtual std::string getBody() const                         = 0;
        };

        class Response
        {
        public:
            virtual ~Response() = default;
            virtual void setHttpStatus(
                HttpStatus status,
                const std::string& optional_reason = "")     = 0;
            virtual void addHeader(const std::string& key,
                                   const std::string& value) = 0;
            // Set the body of the response
            virtual void setBody(const void* data, size_t size) = 0;
            virtual void setBody(const std::string& data)       = 0;
        };

        using RouteHandler = std::function<void(const Request&, Response&)>;

        class Server
        {
        public:
            virtual ~Server() = default;
            virtual [[nodiscard]] std::unique_ptr<Route> addRoute(
                Method method,
                const std::string& uri,
                RouteHandler handler) = 0;
            virtual void start()      = 0;
        };

        std::shared_ptr<Server> createServer(const std::string& ip_address,
                                             const int port);
    }  // namespace server
}  // namespace siesta
