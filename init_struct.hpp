#ifndef INIT_STRUCT_HPP
#define INIT_STRUCT_HPP

#include <memory>
#include "boost/python.hpp"

#include "raw_constructor.hpp"

namespace {
    template<typename T>
        struct num_elements {
            enum { value = 1 };
        };

    template<typename T>
        constexpr std::size_t NumElements = num_elements<T>::value;

    template<typename T, std::size_t N>
        struct num_elements<T[N]> {
            enum {value = N * NumElements<T>};
        };

    //adapted from Xeo's implementation of integer sequence here: http://stackoverflow.com/a/17426611
    template<typename S> using Invoke = typename S::type;

    template<typename... >
        struct type_sequence { using type = type_sequence; };

    template<typename S1, typename S2> struct concat;
    template<typename S1, typename S2> using Concat = Invoke<concat<S1, S2>>;

    template<typename... T1, typename... T2>
        struct concat<type_sequence<T1...>, type_sequence<T2...>> : type_sequence<T1..., T2...> {};

    template<std::size_t N, typename S> struct repeat;
    template<std::size_t N, typename S> using Repeat = Invoke<repeat<N, S>>;

    template<std::size_t N, typename S>
        struct repeat : Concat<Repeat<N/2, S>, Repeat<N - N/2, S>> {};

    template<typename... T>
        struct repeat<0, type_sequence<T...>> : type_sequence<> {};

    template<typename... T>
        struct repeat<1, type_sequence<T...>> : type_sequence<T...> {};

    template<typename S> struct expand;
    template<typename S> using Expand = Invoke<expand<S>>;

    template<>
        struct expand<type_sequence<>> : type_sequence<> {};

    template<typename T, typename... Ts>
        struct expand<type_sequence<T, Ts...>> : Concat<Repeat<NumElements<T>, type_sequence<T>>, Expand<type_sequence<Ts...>>> {};

    template<typename... T> using make_expanded_sequence_for = Expand<type_sequence<T...>>;

    template<typename T1, typename T2> using UseFirstType = T1;
}

template<class T, typename = std::enable_if_t<std::is_pointer<T>::value>>
T populate_helper(boost::python::tuple& args, int& index, boost::python::stl_input_iterator<boost::python::object>&, int&) {
    if ( index < boost::python::len(args) ) {
        if ( !boost::python::object{args[index++]}.is_none() ) {
            PyErr_SetString(PyExc_RuntimeError, "Pointers may not be reliably set from within the constructor at this time; instead, substitute 'None' and assign to the member after construction");
            boost::python::throw_error_already_set();
        }
    }
    return {};
}

template<class T, typename = std::enable_if_t<std::is_array<T>::value>>
std::remove_all_extents_t<T> populate_helper(boost::python::tuple& args, int& index, boost::python::stl_input_iterator<boost::python::object>& iter, int& index_into_array) {
    if ( index < boost::python::len(args) ) {
        if ( index_into_array == 0 ) {
            iter = boost::python::stl_input_iterator<boost::python::object>{args[index]};
        }
        if (index_into_array < NumElements<T> - 1) {
            ++index_into_array;
        } else {
            index++;
            index_into_array = 0;
        }
        if ( iter != boost::python::stl_input_iterator<boost::python::object>{} ) {
            return boost::python::extract<std::remove_all_extents_t<T>>{*(iter++)};
        }
    }
    return {};

}

template<class T, typename = std::enable_if_t<!std::is_array<T>::value>, typename = std::enable_if_t<!std::is_pointer<T>::value>>
T populate_helper(boost::python::tuple& args, int& index, boost::python::stl_input_iterator<boost::python::object>&, int&) {
    if ( index < boost::python::len(args) ) {
        return boost::python::extract<T>{args[index++]};
    }
    return {};
}

template<class T, typename... Ts> struct init_c_struct;

template<class T, typename... MemberTypes, typename... ExpandedTypes>
struct init_c_struct<T, type_sequence<MemberTypes...>, type_sequence<ExpandedTypes...>> {
    static std::shared_ptr<T> fn(boost::python::tuple args, boost::python::dict) {
        int index = 0;
        int index_into_array = 0;
        boost::python::stl_input_iterator<boost::python::object> iter{};
        return std::shared_ptr<T>{
            new T{ populate_helper<ExpandedTypes>(args, index, iter, index_into_array)... }
        };
    }
};

template<class T, typename... MemberTypes>
boost::python::object make_init_function( MemberTypes T::* ...members ) {
    return raw_constructor(&init_c_struct<T, type_sequence<MemberTypes...>, make_expanded_sequence_for<MemberTypes...>>::fn, 0);
}

#endif // INIT_STRUCT_HPP
