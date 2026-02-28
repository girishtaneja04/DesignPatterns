#include <iostream>
#include <functional>
#include <memory>
#include <unordered_map>

using namespace std;

class iPrinter
{
    public:
    virtual void print() = 0;
};

class localPrinter : public iPrinter
{
    public:
    virtual void print()
    {
        cout<<"local printer"<<endl;
    }
};

class canonPrinter : public iPrinter
{
    public:
    virtual void print()
    {
        cout<<"canon printer"<<endl;
    }
};

class printManager
{
    printManager() = default;
    printManager(const printManager &pm) = delete;
    printManager& operator=(const printManager& ab) = delete;

    public:
    static printManager& getInstance()
    {
        static printManager instance;
        return instance;
    }

    template<typename T>
    void registerPrinter(string type)
    {
        if(factories_.find(type) == factories_.end())
        {
            factories_[type] = []() -> shared_ptr<iPrinter>{
                return make_shared<T>();  
            };
        }
    }

    shared_ptr<iPrinter> get(string type)
    {
        if(interface_.find(type) != interface_.end())
        {
            return interface_[type];
        }

        if(factories_.find(type) != factories_.end())
        {
            interface_[type] = factories_[type]();
            return interface_[type];
        }

        throw runtime_error("interface & factories not exist, so plz check");
    }

    unordered_map<string, std::function<shared_ptr<iPrinter>()>> factories_;
    unordered_map<string, shared_ptr<iPrinter>> interface_;
};

int main()
{
    printManager::getInstance().registerPrinter<localPrinter>("local");
    printManager::getInstance().registerPrinter<canonPrinter>("canon");

    printManager::getInstance().get("local")->print();
    printManager::getInstance().get("canon")->print();

    return 0;
}
