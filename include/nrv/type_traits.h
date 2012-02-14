#pragma once

template<class T>
struct is_ref
{
	static const bool value = false;
};
template<class T>
struct is_ref<T&>
{
	static const bool value = true;
};
//template<class T>
//struct is_ref<const T&>
//{
//	static const bool value = false;
//};
template<class T>
struct remove_ref
{
	typedef T type;
};
template<class T>
struct remove_ref<T&>
{
	typedef T type;
};
template<class T>
struct add_ptr
{
	typedef T* type;
};
template<class T>
struct add_ptr<T&>
{
	typedef T* type;
};
template<class T>
struct add_ref
{
	typedef T& type;
};
template<class T>
struct add_ref<T&>
{
	typedef T& type;
};
template <typename T>
struct is_const_ref
{
  static const bool value = false;
};

template <typename T>
struct is_const_ref<const T&>
{
  static const bool value = true;
};