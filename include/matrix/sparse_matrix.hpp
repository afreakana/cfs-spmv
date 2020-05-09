#ifndef SPARSE_MATRIX_HPP
#define SPARSE_MATRIX_HPP

#include <random>
#include <string>

#include "csr_matrix.hpp"
#include "utils/platform.hpp"

namespace matrix {
namespace sparse {

// We will use the Abstract Factory design pattern for Sparse Matrices
// We implement the SparseMatrix as a polymorphic base class so the
// client can manipulate different sparse matrix formats through a
// common base class interface
template <typename IndexT, typename ValueT> class SparseMatrix {
public:
  virtual ~SparseMatrix() = 0;
  virtual int nrows() const = 0;
  virtual int ncols() const = 0;
  virtual int nnz() const = 0;
  virtual bool symmetric() const = 0;
  virtual size_t size() const = 0;
  virtual Platform platform() const = 0;
  virtual bool tune(Kernel k, Tuning t = Tuning::Aggressive) = 0;
  // Multiplication with dense vector
  virtual void dense_vector_multiply(ValueT *__restrict y,
                                     const ValueT *__restrict x) = 0;
  // Factory functions
  // Factory method for initializing a sparse matrix from an MMF file on disk
  static SparseMatrix<IndexT, ValueT> *
  create(const string &filename, Format format = Format::csr,
         Platform platform = Platform::cpu) {
    if (format == Format::sss) {
      return new CSRMatrix<IndexT, ValueT>(filename, platform, true);
    } else if (format == Format::hyb) {
      return new CSRMatrix<IndexT, ValueT>(filename, platform, true, true);
    } else {
      return new CSRMatrix<IndexT, ValueT>(filename, platform);
    }
  }
};

template <typename IndexT, typename ValueT>
SparseMatrix<IndexT, ValueT>::~SparseMatrix() = default;

} // end of namespace sparse
} // end of namespace matrix

#endif
