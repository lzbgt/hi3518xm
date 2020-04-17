// Copyright 2018 Daniel Parker
// Distributed under the Boost license, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/danielaparker/jsoncons for latest version

#ifndef JSONCONS_JSON_CURSOR_HPP
#define JSONCONS_JSON_CURSOR_HPP

#include <memory> // std::allocator
#include <string>
#include <vector>
#include <stdexcept>
#include <system_error>
#include <ios>
#include <istream> // std::basic_istream
#include <jsoncons/byte_string.hpp>
#include <jsoncons/config/jsoncons_config.hpp>
#include <jsoncons/json_content_handler.hpp>
#include <jsoncons/json_exception.hpp>
#include <jsoncons/json_parser.hpp>
#include <jsoncons/staj_reader.hpp>
#include <jsoncons/source.hpp>

namespace jsoncons {

template<class CharT,class Src=jsoncons::stream_source<CharT>,class Allocator=std::allocator<char>>
class basic_json_cursor : public basic_staj_reader<CharT>, private virtual ser_context
{
public:
    typedef Src source_type;
    typedef CharT char_type;
    typedef Allocator allocator_type;
    typedef basic_string_view<CharT> string_view_type;
private:
    typedef typename std::allocator_traits<allocator_type>:: template rebind_alloc<CharT> char_allocator_type;
    static constexpr size_t default_max_buffer_length = 16384;

    basic_staj_event_handler<CharT> event_handler_;
    basic_json_parser<CharT,Allocator> parser_;
    source_type source_;
    std::vector<CharT,char_allocator_type> buffer_;
    std::size_t buffer_length_;
    bool eof_;
    bool begin_;

    // Noncopyable and nonmoveable
    basic_json_cursor(const basic_json_cursor&) = delete;
    basic_json_cursor& operator=(const basic_json_cursor&) = delete;

public:

    // Constructors that throw parse exceptions

    template <class Source>
    basic_json_cursor(Source&& source, 
                      const basic_json_decode_options<CharT>& options = basic_json_decode_options<CharT>(),
                      std::function<bool(json_errc,const ser_context&)> err_handler = default_json_parsing(),
                      const Allocator& alloc = Allocator())
        : basic_json_cursor(std::forward<Source>(source), 
                            accept_all,
                            options,
                            err_handler,
                            alloc)
    {
    }

    template <class Source>
    basic_json_cursor(Source&& source, 
                      std::function<bool(const basic_staj_event<CharT>&, const ser_context&)> filter,
                      const basic_json_decode_options<CharT>& options = basic_json_decode_options<CharT>(),
                      std::function<bool(json_errc,const ser_context&)> err_handler = default_json_parsing(),
                      const Allocator& alloc = Allocator(),
                      typename std::enable_if<!std::is_constructible<basic_string_view<CharT>,Source>::value>::type* = 0)
       : event_handler_(filter),
         parser_(options,err_handler,alloc),
         source_(source),
         buffer_(alloc),
         buffer_length_(default_max_buffer_length),
         eof_(false),
         begin_(true)
    {
        buffer_.reserve(buffer_length_);
        if (!done())
        {
            next();
        }
    }

    template <class Source>
    basic_json_cursor(Source&& source, 
                      std::function<bool(const basic_staj_event<CharT>&, const ser_context&)> filter,
                      const basic_json_decode_options<CharT>& options = basic_json_decode_options<CharT>(),
                      std::function<bool(json_errc,const ser_context&)> err_handler = default_json_parsing(),
                      const Allocator& alloc = Allocator(),
                      typename std::enable_if<std::is_constructible<basic_string_view<CharT>,Source>::value>::type* = 0)
       : event_handler_(filter),
         parser_(options, err_handler, alloc),
         buffer_(alloc),
         buffer_length_(0),
         eof_(false),
         begin_(false)
    {
        basic_string_view<CharT> sv(std::forward<Source>(source));
        auto result = unicons::skip_bom(sv.begin(), sv.end());
        if (result.ec != unicons::encoding_errc())
        {
            JSONCONS_THROW(ser_error(result.ec,parser_.line(),parser_.column()));
        }
        std::size_t offset = result.it - sv.begin();
        parser_.update(sv.data()+offset,sv.size()-offset);
        if (!done())
        {
            next();
        }
    }


    // Constructors that set parse error codes
    template <class Source>
    basic_json_cursor(Source&& source, std::error_code& ec)
        : basic_json_cursor(std::forward<Source>(source),
                                 accept_all,
                                 basic_json_decode_options<CharT>(),
                                 default_json_parsing(),
                                 ec)
    {
    }

    template <class Source>
    basic_json_cursor(Source&& source, 
                      const basic_json_decode_options<CharT>& options,
                      std::error_code& ec)
        : basic_json_cursor(std::forward<Source>(source),
                                 accept_all,
                                 options,
                                 default_json_parsing(),
                                 ec)
    {
    }

    template <class Source>
    basic_json_cursor(Source&& source, 
                      const basic_json_decode_options<CharT>& options,
                      std::function<bool(json_errc,const ser_context&)> err_handler,
                      std::error_code& ec)
        : basic_json_cursor(std::forward<Source>(source),
                                 accept_all,
                                 options,
                                 err_handler,
                                 ec)
    {
    }

    template <class Source>
    basic_json_cursor(Source&& source,
                      std::function<bool(const basic_staj_event<CharT>&, const ser_context&)> filter,
                      std::error_code& ec)
        : basic_json_cursor(std::forward<Source>(source),
                                 filter,
                                 basic_json_decode_options<CharT>(),
                                 default_json_parsing(),
                                 ec)
    {
    }

    template <class Source>
    basic_json_cursor(Source&& source, 
                      std::function<bool(const basic_staj_event<CharT>&, const ser_context&)> filter,
                      const basic_json_decode_options<CharT>& options,
                      std::error_code& ec)
        : basic_json_cursor(std::forward<Source>(source),
                            filter,
                            options,
                            default_json_parsing(),
                            ec)
    {
    }

    template <class Source>
    basic_json_cursor(Source&& source, 
                      std::function<bool(const basic_staj_event<CharT>&, const ser_context&)> filter,
                      const basic_json_decode_options<CharT>& options,
                      std::function<bool(json_errc,const ser_context&)> err_handler,
                      std::error_code& ec)
        : basic_json_cursor(std::allocator_arg, Allocator(), 
                            std::forward<Source>(source),
                            filter,
                            options,
                            err_handler,
                            ec)
    {
    }

    template <class Source>
    basic_json_cursor(std::allocator_arg_t, const Allocator& alloc,
                      Source&& source, 
                      std::function<bool(const basic_staj_event<CharT>&, const ser_context&)> filter,
                      const basic_json_decode_options<CharT>& options,
                      std::function<bool(json_errc,const ser_context&)> err_handler,
                      std::error_code& ec,
                      typename std::enable_if<!std::is_constructible<basic_string_view<CharT>,Source>::value>::type* = 0)
       : event_handler_(filter),
         parser_(options,err_handler,alloc),
         source_(source),
         eof_(false),
         buffer_(alloc),
         buffer_length_(default_max_buffer_length),
         begin_(true)
    {
        buffer_.reserve(buffer_length_);
        if (!done())
        {
            next(ec);
        }
    }

    template <class Source>
    basic_json_cursor(std::allocator_arg_t, const Allocator& alloc,
                      Source&& source, 
                      std::function<bool(const basic_staj_event<CharT>&, const ser_context&)> filter,
                      const basic_json_decode_options<CharT>& options,
                      std::function<bool(json_errc,const ser_context&)> err_handler,
                      std::error_code& ec,
                      typename std::enable_if<std::is_constructible<basic_string_view<CharT>,Source>::value>::type* = 0)
       : event_handler_(filter),
         parser_(options, err_handler, alloc),
         eof_(false),
         buffer_(alloc),
         buffer_length_(0),
         begin_(false)
    {
        basic_string_view<CharT> sv(std::forward<Source>(source));
        auto result = unicons::skip_bom(sv.begin(), sv.end());
        if (result.ec != unicons::encoding_errc())
        {
            ec = result.ec;
            return;
        }
        std::size_t offset = result.it - sv.begin();
        parser_.update(sv.data()+offset,sv.size()-offset);
        if (!done())
        {
            next(ec);
        }
    }

    std::size_t buffer_length() const
    {
        return buffer_length_;
    }

    void buffer_length(std::size_t length)
    {
        buffer_length_ = length;
        buffer_.reserve(buffer_length_);
    }

    bool done() const override
    {
        return parser_.done();
    }

    const basic_staj_event<CharT>& current() const override
    {
        return event_handler_.event();
    }

    void read(basic_json_content_handler<CharT>& handler) override
    {
        std::error_code ec;
        read(handler, ec);
        if (ec)
        {
            JSONCONS_THROW(ser_error(ec,parser_.line(),parser_.column()));
        }
    }

    void read(basic_json_content_handler<CharT>& handler,
              std::error_code& ec) override
    {
        if (!staj_to_saj_event(event_handler_.event(), handler, *this, ec))
        {
            return;
        }
        read_next(handler, ec);
    }

    void next() override
    {
        std::error_code ec;
        next(ec);
        if (ec)
        {
            JSONCONS_THROW(ser_error(ec,parser_.line(),parser_.column()));
        }
    }

    void next(std::error_code& ec) override
    {
        read_next(ec);
    }

    static bool accept_all(const basic_staj_event<CharT>&, const ser_context&) 
    {
        return true;
    }

    void read_buffer(std::error_code& ec)
    {
        buffer_.clear();
        buffer_.resize(buffer_length_);
        std::size_t count = source_.read(buffer_.data(), buffer_length_);
        buffer_.resize(static_cast<std::size_t>(count));
        if (buffer_.size() == 0)
        {
            eof_ = true;
        }
        else if (begin_)
        {
            auto result = unicons::skip_bom(buffer_.begin(), buffer_.end());
            if (result.ec != unicons::encoding_errc())
            {
                ec = result.ec;
                return;
            }
            std::size_t offset = result.it - buffer_.begin();
            parser_.update(buffer_.data()+offset,buffer_.size()-offset);
            begin_ = false;
        }
        else
        {
            parser_.update(buffer_.data(),buffer_.size());
        }
    }

    void read_next(std::error_code& ec)
    {
        read_next(event_handler_, ec);
    }

    void read_next(basic_json_content_handler<CharT>& handler, std::error_code& ec)
    {
        parser_.restart();
        while (!parser_.stopped())
        {
            if (parser_.source_exhausted())
            {
                if (!source_.eof())
                {
                    read_buffer(ec);
                    if (ec) return;
                }
                else
                {
                    eof_ = true;
                }
            }
            parser_.parse_some(handler, ec);
            if (ec) return;
        }
    }

    void check_done()
    {
        std::error_code ec;
        check_done(ec);
        if (ec)
        {
            JSONCONS_THROW(ser_error(ec,parser_.line(),parser_.column()));
        }
    }

    const ser_context& context() const override
    {
        return *this;
    }

    void check_done(std::error_code& ec)
    {
        if (source_.is_error())
        {
            ec = json_errc::source_error;
            return;
        }   
        if (eof_)
        {
            parser_.check_done(ec);
            if (ec) return;
        }
        else
        {
            while (!eof_)
            {
                if (parser_.source_exhausted())
                {
                    if (!source_.eof())
                    {
                        read_buffer(ec);     
                        if (ec) return;
                    }
                    else
                    {
                        eof_ = true;
                    }
                }
                if (!eof_)
                {
                    parser_.check_done(ec);
                    if (ec) return;
                }
            }
        }
    }

    bool eof() const
    {
        return eof_;
    }

    std::size_t line() const override
    {
        return parser_.line();
    }

    std::size_t column() const override
    {
        return parser_.column();
    }

#if !defined(JSONCONS_NO_DEPRECATED)
    JSONCONS_DEPRECATED_MSG("Instead, use read(basic_json_content_handler<CharT>&)")
    void read_to(basic_json_content_handler<CharT>& handler)
    {
        read(handler);
    }

    JSONCONS_DEPRECATED_MSG("Instead, use read(basic_json_content_handler<CharT>&, std::error_code&)")
    void read_to(basic_json_content_handler<CharT>& handler,
                 std::error_code& ec)
    {
        read(handler, ec);
    }
#endif
private:
};

typedef basic_json_cursor<char> json_cursor;
typedef basic_json_cursor<wchar_t> wjson_cursor;

#if !defined(JSONCONS_NO_DEPRECATED)
template<class CharT,class Src,class Allocator=std::allocator<CharT>>
using basic_json_pull_reader = basic_json_cursor<CharT,Src,Allocator>;

JSONCONS_DEPRECATED_MSG("Instead, use json_cursor") typedef json_cursor json_pull_reader;
JSONCONS_DEPRECATED_MSG("Instead, use wjson_cursor") typedef wjson_cursor wjson_pull_reader;

template<class CharT,class Src,class Allocator=std::allocator<CharT>>
using basic_json_stream_reader = basic_json_cursor<CharT,Src,Allocator>;

template<class CharT,class Src,class Allocator=std::allocator<CharT>>
using basic_json_staj_reader = basic_json_cursor<CharT,Src,Allocator>;

JSONCONS_DEPRECATED_MSG("Instead, use json_cursor") typedef json_cursor json_stream_reader;
JSONCONS_DEPRECATED_MSG("Instead, use wjson_cursor") typedef wjson_cursor wjson_stream_reader;

JSONCONS_DEPRECATED_MSG("Instead, use json_cursor") typedef json_cursor json_staj_reader;
JSONCONS_DEPRECATED_MSG("Instead, use wjson_cursor") typedef wjson_cursor wjson_staj_reader;
#endif

}

#endif

