#include <iostream>
#include <vector>


using std::cout;
using std::endl;
using std::string;
using std::vector;

enum class Color {red, green, blue};
enum class Size {small, medium, large};

struct Product
{
    string name;
    Color color;
    Size size;
};

struct Base
{
    virtual bool isSatisfied(Product* item) = 0;
};

template <typename T>
struct Spec: Base
{
    T spec;
    Spec(T s): spec(s){}
};

struct ColorSpec: Spec<Color>
{
    ColorSpec(Color color): Spec<Color>(color){}
    bool isSatisfied(Product *item) override
    {
        return (item->color == this->spec);
    }
};

struct SizeSpec: Spec<Size>
{
    SizeSpec(Size size): Spec<Size>(size){}
    bool isSatisfied(Product *item) override
    {
        return (item->size == this->spec);
    }
};

template <typename T, typename... Args>
class Filter
{
    virtual vector<T*> filter(vector<T*> items,
                              Args*... args) = 0;
};

template <typename... Args>
struct ProductFilter: Filter<Product, Args...>
{
    vector<Product*> filter(vector<Product*> items,
                            Args*... args)
    {
        vector<Base*> specs{args...};
        vector<Product*> result;
        for (auto &item: items)
        {
            bool flag = true;
            for (auto &spec: specs)
            {
                flag &= spec->isSatisfied(item);
            }
            if (flag) result.push_back(item);
        }
        return result;
    }
};


int main(int argc, const char * argv[])
{
    Product apple{"Apple", Color::green, Size::small};
    Product tree{"Tree", Color::green, Size::large};
    Product house{"House", Color::blue, Size::large};
    const vector<Product*> all { &apple, &tree, &house };
    
    ColorSpec green{Color::green};
    SizeSpec large{Size::large};
    
    ProductFilter<ColorSpec> filter;
    for (auto &item: filter.filter(all, &green))
    {
        cout << item->name << " is green and large" << endl;
    }
    return 0;
}
