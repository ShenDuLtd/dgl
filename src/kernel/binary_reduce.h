/*!
 *  Copyright (c) 2019 by Contributors
 * \file kernel/binary_reduce.h
 * \brief Binary reduce function C++ header.
 */
#ifndef DGL_KERNEL_BINARY_REDUCE_H_
#define DGL_KERNEL_BINARY_REDUCE_H_

#include <dgl/runtime/ndarray.h>

#include <vector>
#include <string>

#include "./binary_reduce_common.h"

namespace dgl {
namespace kernel {

// Structure for broadcasting shapes
struct BcastInfo {
  // inferred output shape
  std::vector<int64_t> real_out_shape;
  // Following shapes here have been preprocessed, so that:
  //  - The first dimension (for graph) is removed. Shapes here are only for features.
  //  - They have the same number of dimensions.
  //    e.g. (4,) and (3, 4) become (1, 4) and (3, 4)
  //  - Continuous non-broadcasting dimenions are flattened.
  //    e.g. (4, 1, 3, 3) and (4, 5, 3, 3) become (4, 1, 9) and (4, 5, 9)
  std::vector<int64_t> lhs_shape, lhs_stride;
  std::vector<int64_t> rhs_shape, rhs_stride;
  std::vector<int64_t> out_shape, out_stride;
};

/*
 * !\brief Compute the feature shape after binary reduce computation.
 */
std::vector<int64_t> InferBinaryFeatureShape(
    runtime::NDArray lhs,
    runtime::NDArray rhs);

/*
 * !\brief Multiply src node data with edge data and perform reduce
 *
 * \param reducer The type of the reducer ("sum", "max", "mean", "min", "none").
 *                If the reducer is "none", the output is an edge feature tensor.
 *                Otherwise, a node feature tensor is returned.
 * \param binary_op The type of the binary operator ("mul", "add").
 * \param indptr An int64 row offset array for the graph CSR.
 * \param indices An int64 column index array for the graph CSR.
 * \param rev_indptr An int64 row offset array for the reverse graph CSR.
 * \param rev_indices An int64 column index array for the reverse graph CSR.
 * \param src_mapping An optional int64 array for source node mapping. If empty,
 *                    source ids are consecutive integers [0, len(indptr) - 1).
 *                    Source ids are used to read source node data.
 * \param edge_mapping An optional int64 array for edge mapping. If empty,
 *                     the edge ids are consecutive integers [0, len(indices)).
 *                     The edge ids are used to read edge data.
 * \param src_data The source node feature tensor.
 * \param edge_data The edge feature tensor.
 * \param out_mapping An optional int64 array for output mapping. If reducer is
 *                    "none", then it's a mapping to edge ids. Otherwise, it's
 *                    mapping to destination node ids.
 * \param out_size An integer indicating the output size. If reducer is "none",
 *                 it is the number of output edges. Otherwise it's the number
 *                 of output nodes.
 * \param out_data The output tensor. Could be either node or edge feature
 *                  tensor depending on the reducer.
 */
void SrcOpEdgeReduce(
    const std::string& reducer,
    const std::string& binary_op,
    runtime::NDArray indptr, runtime::NDArray indices,
    runtime::NDArray rev_indptr, runtime::NDArray rev_indices,
    runtime::NDArray src_mapping,
    runtime::NDArray edge_mapping,
    runtime::NDArray src_data,
    runtime::NDArray edge_data,
    runtime::NDArray out_mapping,
    runtime::NDArray out_data);

/*
 * !\brief Multiply src node data with dst node data and perform reduce
 *
 * \param reducer The type of the reducer ("sum", "max", "mean", "min", "none").
 *                If the reducer is "none", the output is an edge feature tensor.
 *                Otherwise, a node feature tensor is returned.
 * \param binary_op The type of the mul functor ("mul", "add").
 * \param indptr An int64 row offset array for the graph CSR.
 * \param indices An int64 column index array for the graph CSR.
 * \param rev_indptr An int64 row offset array for the reverse graph CSR.
 * \param rev_indices An int64 column index array for the reverse graph CSR.
 * \param src_mapping An optional int64 array for source node mapping. If empty,
 *                    source ids are consecutive integers [0, len(indptr) - 1).
 *                    Source ids are used to read source node data.
 * \param dst_mapping An optional int64 array for destination node mapping.
 *                    If empty, the destination ids are consecutive integers
 *                    [0, len(indptr) - 1). The destination ids are used to
 *                    read destination node data.
 * \param src_data The source node feature tensor.
 * \param dst_data The destination node feature tensor.
 * \param out_mapping An optional int64 array for output mapping. If reducer is
 *                    "none", then it's a mapping to edge ids. Otherwise, it's
 *                    mapping to destination node ids.
 * \param out_size An integer indicating the output size. If reducer is "none",
 *                 it is the number of output edges. Otherwise it's the number
 *                 of output nodes.
 * \param out_data The output tensor. Could be either node or edge feature tensor
 *                  depending on the reducer.
 */
void SrcOpDstReduce(
    const std::string& reducer,
    const std::string& binary_op,
    runtime::NDArray indptr, runtime::NDArray indices,
    runtime::NDArray rev_indptr, runtime::NDArray rev_indices,
    runtime::NDArray src_mapping,
    runtime::NDArray dst_mapping,
    runtime::NDArray src_data,
    runtime::NDArray dst_data,
    runtime::NDArray out_mapping,
    runtime::NDArray out_data);

/*
 * !\brief Copy src node data and perform reduce
 
 * \param reducer The type of the reducer ("sum", "max", "mean", "min", "none").
 *                If the reducer is "none", the output is an edge feature tensor.
 *                Otherwise, a node feature tensor is returned.
 * \param indptr An int64 row offset array for the graph CSR.
 * \param indices An int64 column index array for the graph CSR.
 * \param rev_indptr An int64 row offset array for the reverse graph CSR.
 * \param rev_indices An int64 column index array for the reverse graph CSR.
 * \param src_mapping An optional int64 array for source node mapping. If empty,
 *                    source ids are consecutive integers [0, len(indptr) - 1).
 *                    Source ids are used to read source node data.
 * \param src_data The source node feature tensor.
 * \param out_mapping An optional int64 array for output mapping. If reducer is
 *                    "none", then it's a mapping to edge ids. Otherwise, it's
 *                    mapping to destination node ids.
 * \param out_size An integer indicating the output size. If reducer is "none",
 *                 it is the number of output edges. Otherwise it's the number
 *                 of output nodes.
 * \param out_data The output tensor. Could be either node or edge feature tensor
 *                  depending on the reducer.
 *
 */
void CopySrcReduce(
    const std::string& reducer,
    runtime::NDArray indptr, runtime::NDArray indices,
    runtime::NDArray rev_indptr, runtime::NDArray rev_indices,
    runtime::NDArray src_mapping,
    runtime::NDArray src_data,
    runtime::NDArray out_mapping,
    runtime::NDArray out_data);

/*
 * !\brief Copy edge data and perform reduce
 
 * \param reducer The type of the reducer ("sum", "max", "mean", "min", "none").
 *                If the reducer is "none", the output is an edge feature tensor.
 *                Otherwise, a node feature tensor is returned.
 * \param indptr An int64 row offset array for the graph CSR.
 * \param indices An int64 column index array for the graph CSR.
 * \param rev_indptr An int64 row offset array for the reverse graph CSR.
 * \param rev_indices An int64 column index array for the reverse graph CSR.
 * \param edge_mapping An optional int64 array for source node mapping. If empty,
 *                    source ids are consecutive integers [0, len(indptr) - 1).
 *                    Source ids are used to read source node data.
 * \param edge_data The source node feature tensor.
 * \param out_mapping An optional int64 array for output mapping. If reducer is
 *                    "none", then it's a mapping to edge ids. Otherwise, it's
 *                    mapping to destination node ids.
 * \param out_size An integer indicating the output size. If reducer is "none",
 *                 it is the number of output edges. Otherwise it's the number
 *                 of output nodes.
 * \param out_data The output tensor. Could be either node or edge feature tensor
 *                  depending on the reducer.
 *
 */
void CopyEdgeReduce(
    const std::string& reducer,
    runtime::NDArray indptr, runtime::NDArray indices,
    runtime::NDArray rev_indptr, runtime::NDArray rev_indices,
    runtime::NDArray edge_mapping,
    runtime::NDArray edge_data,
    runtime::NDArray out_mapping,
    runtime::NDArray out_data);

/*
 * !\brief Backward operator for SrcOpEdgeReduce. Compute the gradient for the src data.
 *
 * \param reducer The type of the reducer ("sum", "max", "mean", "min", "none").
 *                If the reducer is "none", the output is an edge feature tensor.
 *                Otherwise, a node feature tensor is returned.
 * \param binary_op The type of the binary operator ("mul", "add").
 * \param indptr An int64 row offset array for the graph CSR.
 * \param indices An int64 column index array for the graph CSR.
 * \param rev_indptr An int64 row offset array for the reverse graph CSR.
 * \param rev_indices An int64 column index array for the reverse graph CSR.
 * \param src_mapping An optional int64 array for source node mapping. If empty,
 *                    source ids are consecutive integers [0, len(indptr) - 1).
 *                    Source ids are used to read source node data.
 * \param edge_mapping An optional int64 array for edge mapping. If empty,
 *                     the edge ids are consecutive integers [0, len(indices)).
 *                     The edge ids are used to read edge data.
 * \param out_mapping An optional int64 array for output mapping. If reducer is
 *                    "none", then it's a mapping to edge ids. Otherwise, it's
 *                    mapping to destination node ids.
 * \param src_data The source node feature tensor.
 * \param edge_data The edge feature tensor.
 * \param out_data The forward output tensor. Could be either node or edge feature
 *                 tensor depending on the reducer.
 * \param grad_out_data The gradient tensor fo the output.
 * \param The gradient tensor of the src data.
 */
void BackwardLhsSrcOpEdgeReduce(
    const std::string& reducer,
    const std::string& binary_op,
    runtime::NDArray indptr, runtime::NDArray indices,
    runtime::NDArray rev_indptr, runtime::NDArray rev_indices,
    runtime::NDArray src_mapping,
    runtime::NDArray edge_mapping,
    runtime::NDArray out_mapping,
    runtime::NDArray src_data,
    runtime::NDArray edge_data,
    runtime::NDArray out_data,
    runtime::NDArray grad_out_data,
    runtime::NDArray grad_lhs_data);
/*
 * !\brief Backward operator for SrcOpEdgeReduce. Compute the gradient for the edge data.
 *
 * \param reducer The type of the reducer ("sum", "max", "mean", "min", "none").
 *                If the reducer is "none", the output is an edge feature tensor.
 *                Otherwise, a node feature tensor is returned.
 * \param binary_op The type of the binary operator ("mul", "add").
 * \param indptr An int64 row offset array for the graph CSR.
 * \param indices An int64 column index array for the graph CSR.
 * \param rev_indptr An int64 row offset array for the reverse graph CSR.
 * \param rev_indices An int64 column index array for the reverse graph CSR.
 * \param src_mapping An optional int64 array for source node mapping. If empty,
 *                    source ids are consecutive integers [0, len(indptr) - 1).
 *                    Source ids are used to read source node data.
 * \param edge_mapping An optional int64 array for edge mapping. If empty,
 *                     the edge ids are consecutive integers [0, len(indices)).
 *                     The edge ids are used to read edge data.
 * \param out_mapping An optional int64 array for output mapping. If reducer is
 *                    "none", then it's a mapping to edge ids. Otherwise, it's
 *                    mapping to destination node ids.
 * \param src_data The source node feature tensor.
 * \param edge_data The edge feature tensor.
 * \param out_data The forward output tensor. Could be either node or edge feature
 *                 tensor depending on the reducer.
 * \param grad_out_data The gradient tensor fo the output.
 * \param The gradient tensor of the edge data.
 */
void BackwardRhsSrcOpEdgeReduce(
    const std::string& reducer,
    const std::string& binary_op,
    runtime::NDArray indptr, runtime::NDArray indices,
    runtime::NDArray rev_indptr, runtime::NDArray rev_indices,
    runtime::NDArray src_mapping,
    runtime::NDArray edge_mapping,
    runtime::NDArray out_mapping,
    runtime::NDArray src_data,
    runtime::NDArray edge_data,
    runtime::NDArray out_data,
    runtime::NDArray grad_out_data,
    runtime::NDArray grad_rhs_data);
/*
 * !\brief Backward operator for SrcOpEdgeReduce. Compute both gradients in one kernel.
 *
 * \param reducer The type of the reducer ("sum", "max", "mean", "min", "none").
 *                If the reducer is "none", the output is an edge feature tensor.
 *                Otherwise, a node feature tensor is returned.
 * \param binary_op The type of the binary operator ("mul", "add").
 * \param indptr An int64 row offset array for the graph CSR.
 * \param indices An int64 column index array for the graph CSR.
 * \param rev_indptr An int64 row offset array for the reverse graph CSR.
 * \param rev_indices An int64 column index array for the reverse graph CSR.
 * \param src_mapping An optional int64 array for source node mapping. If empty,
 *                    source ids are consecutive integers [0, len(indptr) - 1).
 *                    Source ids are used to read source node data.
 * \param edge_mapping An optional int64 array for edge mapping. If empty,
 *                     the edge ids are consecutive integers [0, len(indices)).
 *                     The edge ids are used to read edge data.
 * \param out_mapping An optional int64 array for output mapping. If reducer is
 *                    "none", then it's a mapping to edge ids. Otherwise, it's
 *                    mapping to destination node ids.
 * \param src_data The source node feature tensor.
 * \param edge_data The edge feature tensor.
 * \param out_data The forward output tensor. Could be either node or edge feature
 *                 tensor depending on the reducer.
 * \param grad_out_data The gradient tensor fo the output.
 * \param The gradient tensor of the src data.
 * \param The gradient tensor of the edge data.
 */
void BackwardBothSrcOpEdgeReduce(
    const std::string& reducer,
    const std::string& binary_op,
    runtime::NDArray indptr, runtime::NDArray indices,
    runtime::NDArray rev_indptr, runtime::NDArray rev_indices,
    runtime::NDArray src_mapping,
    runtime::NDArray edge_mapping,
    runtime::NDArray out_mapping,
    runtime::NDArray src_data,
    runtime::NDArray edge_data,
    runtime::NDArray out_data,
    runtime::NDArray grad_out_data,
    runtime::NDArray grad_lhs_data,
    runtime::NDArray grad_rhs_data);

}  // namespace kernel
}  // namespace dgl

#endif  // DGL_KERNEL_BINARY_REDUCE_H_
