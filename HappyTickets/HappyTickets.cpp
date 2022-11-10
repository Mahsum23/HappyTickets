#include <iostream>
#include <string>
#include <regex>
#include <filesystem>
#include <functional>
#include <fstream>
#include <numeric>

namespace fs = std::filesystem;
using namespace std::literals;


class Tester
{
public:

    Tester(const std::string& in_ext, const std::string& out_ext, std::function<std::string(std::string)> function)
        : in_ext_(in_ext), out_ext_(out_ext), function_(function)
    {

    }

    void Run()
    {
        for (const auto& file : fs::directory_iterator(fs::current_path()))
        {
            std::string in_filename = file.path().string();
            if (file.path().filename().string().find(in_ext_) != std::string::npos)
            {
                std::ifstream in_file(file.path().string());
                std::string in_line;
                getline(in_file, in_line);
                
                std::string out_filename = file.path().filename().stem().string() + out_ext_;
                std::ifstream out_file(out_filename);
                std::string out_line;
                getline(out_file, out_line);
                
                if ((function_(in_line)) == out_line)
                {
                    std::cout << file.path().filename() << ": " << "PASSED" << std::endl << std::endl;
                }
                else
                {
                    std::cout << file.path().filename() << ": " << "FAILED" << std::endl;
                    std::cout << "\tINPUT: " << in_line << std::endl;
                    std::cout << '\t' << "SHOULD BE: " << out_line << "\n\tRESULT: " << function_(in_line) << std::endl << std::endl;
                }
                
            }
        }
    }

private:
    std::string in_ext_;
    std::string out_ext_;
    std::function<std::string(std::string)> function_;
};

std::vector<size_t> VectorShiftSum(const std::vector<size_t>&v, size_t shift)
{
    std::vector<size_t> res(v.size() + shift);
    size_t sum = v[0];
    size_t cur_sum_forw = 0;
    size_t cur_sum_back = 0;

    for (size_t i = 0; i < std::min(shift + 1, v.size()); ++i)
    {
        cur_sum_forw += v[i];
        cur_sum_back += v[v.size() - i - 1];
        res[i] = cur_sum_forw;
        res[res.size() - i - 1] = cur_sum_back;
    }
    for (size_t i = v.size(); i < shift; ++i)
    {
        res[i] = res[i - 1];
    }
    for (size_t i = shift + 1; i < v.size(); ++i)
    {
        res[i] = res[i - 1] - v[i - shift - 1] + v[i];
    }
    return res;
}

size_t HappyTicket(int n)
{
    std::vector<size_t> v(10, 1);
    for (int i = 2; i <= n; ++i)
    {
        v = VectorShiftSum(v, 9);
    }
    return std::inner_product(v.begin(), v.end(), v.begin(), 0ull);
}

int main()
{
    std::string in_ext = ".in";
    std::string out_ext = ".out";
    
    Tester tester(in_ext, out_ext, std::function<std::string(std::string)>([](std::string str)
        { 
            return std::to_string(HappyTicket(stoi(str)));
        }));

    tester.Run();
    
    std::cin.get();
}
