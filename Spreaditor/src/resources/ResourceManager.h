#pragma once

#include "../utils/Manager.h"

#include <unordered_map>
#include <string>
#include <memory>
#include <cassert>
#include <sstream>

#include "File.h"
#include "../debug/LoggingManager.h"


template<typename M, typename R>
struct ScopedResource {

    ScopedResource(const std::string& filename) {
        m_filename = filename;
        resource = M::get().get_required_resource(m_filename);
    }

    ~ScopedResource() {
        M::get().release_required_resource(m_filename);
    }

    std::string m_filename;
    R* resource;
};

template<typename T, typename R>
class ResourceManager : public Manager<T>
{
    friend class CRSP<T>;

    using ResourceTuple = std::tuple<std::unique_ptr<R>, unsigned int>;
    using ResourceMap = std::unordered_map<std::string, ResourceTuple>;

public:

    virtual ~ResourceManager() {
        /*
        This should have been cleaned up with shutdown
        */
        assert(m_resources.size() == 0);
    }

    void start_up() {

    }

    void shut_down() {
        /*
        We still have some resources in use!
        */
        if (m_resources.size() != 0) {
            for (const auto& res : m_resources) {
                auto ss = std::stringstream();
                ss << res.first <<
                    " is still in use at the end of execution with " <<
                    std::get<1>(res.second) << " elements!\n";
                LOGF(ss.str());
            }
        }

        m_resources.clear();
    }

    bool exists_resource(const std::string& filename) {
        auto it = m_resources.find(filename);
        if (it == m_resources.end()) {
            return file_exists(filename.c_str());
        }
        else {
            return true;
        }
    }

    R* get_resource(const std::string& filename) {
        auto it = m_resources.find(filename);
        if (it == m_resources.end()) {
            return load_and_store_resource(filename);
        }
        else {
            return std::get<0>(it->second).get();
        }
    }

    R* get_required_resource(const std::string& filename) {
        auto it = m_resources.find(filename);
        if (it == m_resources.end()) {
            return load_and_store_resource(filename, true);
        }
        else {
            ++std::get<1>(it->second);
            return std::get<0>(it->second).get();
        }
    }

    ScopedResource<T, R> get_scoped_resource(const std::string& filename) {
        return ScopedResource<T, R>(filename);
    }

    bool release_required_resource(const std::string& filename) {
        auto it = m_resources.find(filename);
        if (it == m_resources.end()) {
            return false;
        }
        else {
            auto count = --std::get<1>(it->second);
            if (count == 0) {
                m_resources.erase(it);
            }
            return true;
        }
    }

private:

    virtual R* load_resource(const std::string & filename) = 0;

    R * load_and_store_resource(const std::string & filename, bool required = false) {

        auto res = load_resource(filename);
        auto uses = 0u;
        if (required) uses = 1;
        m_resources[filename] = std::make_tuple(std::unique_ptr<R>(res), uses);
        return res;
    }

    ResourceMap m_resources;

};

