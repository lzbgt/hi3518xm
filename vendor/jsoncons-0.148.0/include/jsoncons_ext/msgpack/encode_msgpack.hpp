// Copyright 2013 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_MSGPACK_ENCODE_MSGPACK_HPP
#define JSONCONS_MSGPACK_ENCODE_MSGPACK_HPP

#include <string>
#include <vector>
#include <memory>
#include <type_traits> // std::enable_if
#include <istream> // std::basic_istream
#include <jsoncons/json.hpp>
#include <jsoncons/config/jsoncons_config.hpp>
#include <jsoncons_ext/msgpack/msgpack_encoder.hpp>
#include <jsoncons_ext/msgpack/msgpack_reader.hpp>

namespace jsoncons { 
namespace msgpack {

    template<class T>
    typename std::enable_if<is_basic_json_class<T>::value,void>::type 
    encode_msgpack(const T& j, std::vector<uint8_t>& v)
    {
        typedef typename T::char_type char_type;
        msgpack_bytes_encoder encoder(v);
        auto adaptor = make_json_content_handler_adaptor<basic_json_content_handler<char_type>>(encoder);
        j.dump(adaptor);
    }

    template<class T>
    typename std::enable_if<!is_basic_json_class<T>::value,void>::type 
    encode_msgpack(const T& val, std::vector<uint8_t>& v)
    {
        msgpack_bytes_encoder encoder(v);
        std::error_code ec;
        ser_traits<T>::serialize(val, encoder, json(), ec);
        if (ec)
        {
            JSONCONS_THROW(ser_error(ec));
        }
    }

    template<class T>
    typename std::enable_if<is_basic_json_class<T>::value,void>::type 
    encode_msgpack(const T& j, std::ostream& os)
    {
        typedef typename T::char_type char_type;
        msgpack_stream_encoder encoder(os);
        auto adaptor = make_json_content_handler_adaptor<basic_json_content_handler<char_type>>(encoder);
        j.dump(adaptor);
    }

    template<class T>
    typename std::enable_if<!is_basic_json_class<T>::value,void>::type 
    encode_msgpack(const T& val, std::ostream& os)
    {
        msgpack_stream_encoder encoder(os);
        std::error_code ec;
        ser_traits<T>::serialize(val, encoder, json(), ec);
        if (ec)
        {
            JSONCONS_THROW(ser_error(ec));
        }
    }

    // with temp_allocator_arg_t

    template<class T,class TempAllocator>
    typename std::enable_if<is_basic_json_class<T>::value,void>::type 
    encode_msgpack(temp_allocator_arg_t, const TempAllocator& temp_alloc, const T& j, std::vector<uint8_t>& v)
    {
        typedef typename T::char_type char_type;
        basic_msgpack_encoder<jsoncons::bytes_sink,TempAllocator> encoder(v, temp_alloc);
        auto adaptor = make_json_content_handler_adaptor<basic_json_content_handler<char_type>>(encoder);
        j.dump(adaptor);
    }

    template<class T,class TempAllocator>
    typename std::enable_if<!is_basic_json_class<T>::value,void>::type 
    encode_msgpack(temp_allocator_arg_t, const TempAllocator& temp_alloc, const T& val, std::vector<uint8_t>& v)
    {
        basic_msgpack_encoder<jsoncons::bytes_sink,TempAllocator> encoder(v, temp_alloc);
        std::error_code ec;
        ser_traits<T>::serialize(val, encoder, json(), ec);
        if (ec)
        {
            JSONCONS_THROW(ser_error(ec));
        }
    }

    template<class T,class TempAllocator>
    typename std::enable_if<is_basic_json_class<T>::value,void>::type 
    encode_msgpack(temp_allocator_arg_t, const TempAllocator& temp_alloc, const T& j, std::ostream& os)
    {
        typedef typename T::char_type char_type;
        basic_msgpack_encoder<jsoncons::binary_stream_sink,TempAllocator> encoder(os, temp_alloc);
        auto adaptor = make_json_content_handler_adaptor<basic_json_content_handler<char_type>>(encoder);
        j.dump(adaptor);
    }

    template<class T,class TempAllocator>
    typename std::enable_if<!is_basic_json_class<T>::value,void>::type 
    encode_msgpack(temp_allocator_arg_t, const TempAllocator& temp_alloc, const T& val, std::ostream& os)
    {
        basic_msgpack_encoder<jsoncons::binary_stream_sink,TempAllocator> encoder(os, temp_alloc);
        std::error_code ec;
        ser_traits<T>::serialize(val, encoder, json(), ec);
        if (ec)
        {
            JSONCONS_THROW(ser_error(ec));
        }
    }

} // msgpack
} // jsoncons

#endif
