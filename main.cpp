#include "include/hc.hpp"

fun {
    let<int> arr1 = 10;
    let<double> arr2 = 3.14;

    echo("arr1: " << *arr1);
    echo("arr2: " << *arr2);

    *arr1 = 20;
    *arr2 = 6.28;

    echo("arr1 (after modification): " << *arr1);
    echo("arr2 (after modification): " << *arr2);

    return 0;
}
