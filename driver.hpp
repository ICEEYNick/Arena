#pragma once 
#include <windows.h>
#include <cstdint> 
#include <mutex> 
#include <vector>
#include <locale>         // std::wstring_convert
#include <codecvt>        // std::codecvt_utf8
#include <cstdint>        // std::uint_least32_t
#include <TlHelp32.h>
#include <tchar.h>
#include "xor.h"
class driver {
public:
    static void initialize();
    static void set_pid(const std::uint32_t pid);
    static std::uint32_t get_pid(const wchar_t* process_name);
    static std::uint64_t get_modulebase(const wchar_t* module_name, const std::uint32_t pid = game_pid);
    static void read_memory(void* address, void* buffer, const std::size_t size);
    static void write_memory(void* address, void* buffer, const std::size_t size);
    template <typename type>
    inline static const type& read(const std::uint64_t address) {
        type buffer{};
        read_memory((void*)address, &buffer, sizeof(buffer));
        return buffer;
    }
    template <typename type>
    inline static void write(const std::uint64_t address, const type& value) {
        write_memory((void*)address, (void*)&value, sizeof(value));
    }

    static uint64_t ReadChain(uint64_t base, const std::vector<uint64_t>& offsets) {
        uint64_t result = driver::read<uint64_t>(base + offsets.at(0));
        for (int i = 1; i < offsets.size(); i++) {
            result = driver::read<uint64_t>(result + offsets.at(i));
        }
        return result;
    }

    static std::wstring read_unicode(const std::uintptr_t address, std::size_t size)
    {
        const auto buffer = std::make_unique<wchar_t[]>(size);
        driver::read_memory((uint64_t*)address, buffer.get(), size * 2);
        return std::wstring(buffer.get());
    }
    static std::string read_ascii(const std::uintptr_t address, std::size_t size)
    {
        std::unique_ptr<char[]> buffer(new char[size]);
        driver::read_memory((uint64_t*)address, buffer.get(), size);
        return std::string(buffer.get());
    }
    static int32_t GetStringSize(uint64_t addr)
    {
        int32_t len = read<int32_t>(addr + 0x10);
        if (len > 1000 || len < 0)
            return 0;
        else
            return len;
    }
    static std::string GetWString(uint64_t addr)
    {
        std::string result;
        char mem_buffer[0x1000];
        char* buf_runner = mem_buffer;
        int32_t StringSize = GetStringSize(addr) * 2;

        driver::read_memory((uint64_t*)addr + 0x14, mem_buffer, StringSize); // 0x50 for id

        for (int i = 0; i < StringSize; i += 2)
        {
            char16_t CurrentChar = *((char*)buf_runner + (i));
            CurrentChar += (*((char*)buf_runner + (i + 1)) << 8);
            result.push_back(CurrentChar);

        }

        return result;
    }

    static bool in_array(const std::string& value, const std::vector<std::string>& array)
    {
        return std::find(array.begin(), array.end(), value) != array.end();
    }

    template <typename t>
    static t chain(uintptr_t address, std::vector<uintptr_t> chain)
    {
        uintptr_t cur_read = address;

        for (int i = 0; i < chain.size() - 1; ++i)
            cur_read = driver::read<uintptr_t>(cur_read + chain[i]);

        return driver::read<t>(cur_read + chain[chain.size() - 1]);
    }
    static std::string wstring_to_utf8(const std::wstring& str) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
        return myconv.to_bytes(str);
    }
    static std::wstring utf8_to_wstring(const std::string& str) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
        return myconv.from_bytes(str);
    }
    static std::string get_unicode_str(uintptr_t address, size_t size)
    {
        address = address + 0x14;
        char16_t wcharTemp[64] = { '\0' };
        driver::read_memory((uint64_t*)address, (uint8_t*)&wcharTemp, size * 2);
        std::string u8_conv = std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(wcharTemp);
        return u8_conv;
    }

    static void disconnect();
private:
    static void send_request();
private:
#pragma region SHARED_DATA 
    enum class request_e : std::uint8_t {
        NONE = 0,
        GET_PID,
        GET_MODULEBASE,
        READ_MEM,
        WRITE_MEM,
        DISCONNECT
    };
    struct magic_sig_t {
        const std::uint64_t key = 0x44488567FFEui64;
        const std::uint64_t key2 = 0xFFFFFF33735ui64;

        inline const bool operator==(const magic_sig_t& o) {
            return key == o.key && key2 == o.key2;
        }
    };
    struct shared_data_t {
        bool usermode_done = false;
        bool kernelmode_done = false;
        request_e request_type{};
        void* pid{};
        void* address{};
        void* buffer{};
        std::size_t size{};
        const wchar_t* p_wstr{};
    };
#pragma endregion SHARED_DATA 
    struct comm_t {
        magic_sig_t magic_signature;
        shared_data_t** comm_ptr = nullptr;
    };
private:
    inline static constexpr std::uint64_t confirm = 0x1888;
    inline static comm_t comm;
    inline static shared_data_t* shared_data = nullptr;
    inline static std::uint32_t pid{};
    inline static std::uint32_t game_pid{};
    inline static std::mutex mtx;
};
