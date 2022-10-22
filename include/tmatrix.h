// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
//

#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Динамический вектор - 
// шаблонный вектор на динамической памяти

template <typename T>
class TDynamicVector
{
protected:
  size_t sz;
  T* pMem;
public:
  TDynamicVector(size_t size = 1) : sz(size)
  {
    if (sz == 0)
      throw out_of_range("Vector size should be greater than zero");
    if (sz > MAX_VECTOR_SIZE)
      throw out_of_range("Vector size should be less or equal MAX_VECTOR_SIZE");
    pMem = new T[sz]();// {}; // У типа T д.б. констуктор по умолчанию
  }
  TDynamicVector(T* arr, size_t s) : sz(s)
  {
    // assert(arr != nullptr && "TDynamicVector ctor requires non-nullptr arg");
    pMem = new T[sz];
    std::copy(arr, arr + sz, pMem);
  }
  TDynamicVector(const TDynamicVector& v): TDynamicVector(v.pMem, v.sz) {}
  TDynamicVector(TDynamicVector&& v) noexcept
  {
    pMem = v.pMem;
    v.pMem = nullptr;
    sz = v.sz;
  }
  ~TDynamicVector()
  {
    delete[] pMem;
  }
  TDynamicVector& operator=(const TDynamicVector& v)
  {
    if(this != &v)
      *this = TDynamicVector(v);
    return *this;
  }
  TDynamicVector& operator=(TDynamicVector&& v) noexcept
  {
    swap(pMem, v.pMem);
    sz = v.sz;
    return *this;
  }

  size_t size() const noexcept { return sz; }

  // индексация
  T& operator[](size_t ind)
  {
    return pMem[ind];
  }
  const T& operator[](size_t ind) const
  {
    return pMem[ind];
  }
  // индексация с контролем
  T& at(size_t ind)
  {
    if (ind >= sz) {
      throw out_of_range("Index out of range");
    }
    return pMem[ind];
  }
  const T& at(size_t ind) const
  {
    if (ind >= sz) {
      throw out_of_range("Index out of range");
    }
    return pMem[ind];
  }

  // сравнение
  bool operator==(const TDynamicVector& v) const noexcept
  {
    if (this == &v)
      return true;
    if (sz != v.sz)
      return false;
    for (int i = 0; i < sz; i++)
      if (pMem[i] != v.pMem[i])
        return false;
    return true;
  }
  bool operator!=(const TDynamicVector& v) const noexcept
  {
    return !(*this == v);
  }

  // скалярные операции
  TDynamicVector operator+(T val)
  {
    TDynamicVector result(this->sz);
    for (int i = 0; i < sz; i++) {
      result[i] = pMem[i] +  val;
    }
    return result;
  }
  TDynamicVector operator-(T val) // Изменен тип параметра (double val -> T val) , чтобы функция имела смысл
  {
    TDynamicVector result(this->sz);
    for (int i = 0; i < sz; i++) {
      result[i] = pMem[i] - val;
    }
    return result;
  }
  TDynamicVector operator*(T val) // Изменен тип параметра (double val -> T val) , чтобы функция имела смысл
  {
    TDynamicVector result(this->sz);
    for (int i = 0; i < sz; i++) {
      result[i] = pMem[i] * val;
    }
    return result;
  }

  // векторные операции
  TDynamicVector operator+(const TDynamicVector& v)
  {
    if (sz != v.sz)
      throw logic_error("Sum of two vectors with different size is undefined");
    TDynamicVector result(this->sz);
    for (int i = 0; i < sz; i++)
      result.pMem[i] = pMem[i] + v.pMem[i];
    return result;
  }
  TDynamicVector operator-(const TDynamicVector& v)
  {
    if (sz != v.sz)
      throw logic_error("Difference of two vectors with different size is undefined");
    TDynamicVector result(this->sz);
    for (int i = 0; i < sz; i++)
      result.pMem[i] = pMem[i] - v.pMem[i];
    return result;
  }
  T operator*(const TDynamicVector& v) noexcept(noexcept(T()))
  {
    if (sz != v.sz)
      throw logic_error("Product of two vectors with different size is undefined");
    T result = pMem[0] * v.pMem[0];
    for (int i = 1; i < sz; i++) {
      result = result + pMem[i] * v.pMem[i];
    }
    return result;
  }

  friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept
  {
    std::swap(lhs.sz, rhs.sz);
    std::swap(lhs.pMem, rhs.pMem);
  }

  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicVector& v)
  {
    for (size_t i = 0; i < v.sz; i++)
      istr >> v.pMem[i]; // требуется оператор>> для типа T
    return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicVector& v)
  {
    for (size_t i = 0; i < v.sz; i++)
      ostr << v.pMem[i] << ' '; // требуется оператор<< для типа T
    return ostr;
  }
};


// Динамическая квадратная матрица - 
// шаблонная матрица на динамической памяти
template<typename T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>>
{
  using TDynamicVector<TDynamicVector<T>>::pMem;
  using TDynamicVector<TDynamicVector<T>>::sz;
public:
  TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(s) 
    // можно создать только квадратную матрицу
  {
    if (s >= MAX_MATRIX_SIZE)
      throw out_of_range("Matrix size should be less or equal MAX_VECTOR_SIZE");
    for (size_t i = 0; i < sz; i++)
      pMem[i] = TDynamicVector<T>(sz);
  }

  size_t size() const noexcept 
  {
    return sz;
  }

  using TDynamicVector<TDynamicVector<T>>::operator[];
  using TDynamicVector<TDynamicVector<T>>::at;

  // индексация с контролем
  T& at(size_t row, size_t col) {
    return (*this).at(row).at(col);
  }

  // сравнение
  bool operator==(const TDynamicMatrix& m) const noexcept
  {
    for (int i = 0; i < sz; i++) {
      if ((*this)[i] != m[i])
        return false;
    }
    return true;
  }

  // матрично-скалярные операции
  TDynamicMatrix<T> operator*(const T& val)
  {
    TDynamicMatrix res(sz);
    for (int i = 0; i < sz; i++)
      res[i] = (*this)[i] * val;
    return res;
  }

  // матрично-векторные операции
  TDynamicVector<T> operator*(const TDynamicVector<T>& v)
  {
    if (sz != v.sz)
      throw logic_error("Matrix size doesn't match vector size");
    TDynamicVector res(sz);
    for (int i = 0; i < sz; i++) {
      res[i] = (*this)[i] * v;
    }
    return res;
  }

  // матрично-матричные операции
  TDynamicMatrix operator+(const TDynamicMatrix& m)
  {
    TDynamicMatrix res(sz);
    for (int i = 0; i < sz; i++)
      res[i] = (*this)[i] + m[i];
    return res;
  }
  TDynamicMatrix operator-(const TDynamicMatrix& m)
  {
    TDynamicMatrix res(sz);
    for (int i = 0; i < sz; i++)
      res[i] = (* this)[i] - m[i];
    return res;
  }
  TDynamicMatrix operator*(const TDynamicMatrix& m)
  {
    TDynamicMatrix res(sz);
    for (int i = 0; i < sz; i++)
      res[i] = (*this)[i] * m[i];
    return res;
  }

  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicMatrix& m)
  // на самом деле в friend нет необходимости
  {
    for (int i = 0; i < m.size(); i++) {
      for (int j = 0; j < m.size(); j++) {
        istr >> m[i][j];
      }
    }
    return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicMatrix& m)
  {
  // на самом деле в friend нет необходимости
    for (int i = 0; i < m.size(); i++) {
      for (int j = 0; j < m.size(); j++) {
        ostr << m[i][j] << ' ';
      }
      ostr << endl;
    }
    return ostr;
  }
};


#endif
