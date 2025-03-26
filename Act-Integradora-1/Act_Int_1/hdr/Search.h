#ifndef SEARCH_H
#define SEARCH_H

#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::endl;

template <class T>
class Search
{
public: // Constructor que se eliminado para evitar instanciación de la clase.
    Search() = delete;
    [[nodiscard]] static int binary_custom(vector<T>& vals, int low, int high, int k, const int& int_element);
    [[nodiscard]] static vector<int> find_by_interval(vector<T>& vals, const unsigned int& D, const int& int_element);
private:
};

template <class T>
int Search<T>::binary_custom(vector<T>& vals, int low, int high, int k, const int& int_element)
{
    while (low <= high)
    {
        int middle = low + (high - low) / 2;

        if (vals[middle](int_element) == k)
        {

            return middle;
        }

        if (vals[middle](int_element) < k)
        {
            low = middle + 1;
        }
        else
        {
            high = middle - 1;
        }
    }
    return -1;
}

template <class T>
vector<int> Search<T>::find_by_interval(vector<T>& vals, const unsigned int& D, const int& int_element)
{
    vector<int> pair(2, -1);
    int l = 0;
    int r = 1;

    while (l <= r && r < static_cast<int>(vals.size()))
    {
        const int diff = vals[r](int_element) - vals[l](int_element);

        if (diff == static_cast<int>(D) && l != r)
        {
            pair[0] = l;
            pair[1] = r;
            return pair;
        }
        else if (diff > static_cast<int>(D))
        {
            l++;
        }
        else
        {
            r++;
        }
    }
    return pair;
}

#endif //SEARCH_H
