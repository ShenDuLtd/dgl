/*!
 *  Copyright (c) 2019 by Contributors
 * \file kernel/cpu/binary_reduce_impl.cc
 * \brief Binary reduce implementation on CPU.
 */
#include "../binary_reduce.h"
#include "../binary_reduce_impl.h"

using dgl::runtime::NDArray;

namespace dgl {
namespace kernel {

template void BinaryReduceImpl<kDLCPU>(
    const std::string& reducer,
    const std::string& op,
    NDArray indptr, NDArray indices,
    NDArray rev_indptr, NDArray rev_indices,
    binary_op::Target lhs, binary_op::Target rhs,
    NDArray lhs_mapping, NDArray rhs_mapping,
    NDArray lhs_data, NDArray rhs_data,
    NDArray out_mapping, NDArray out_data);

template void BackwardBinaryReduceImpl<kDLCPU>(
    const std::string& reducer,
    const std::string& op,
    NDArray indptr, NDArray indices,
    NDArray rev_indptr, NDArray rev_indices,
    binary_op::Target lhs, binary_op::Target rhs,
    NDArray lhs_mapping, NDArray rhs_mapping, NDArray out_mapping,
    NDArray lhs_data, NDArray rhs_data, NDArray out_data,
    NDArray grad_out_data,
    NDArray grad_lhs_data, NDArray grad_rhs_data);

template void BinaryReduceBcastImpl<kDLCPU>(
    const BcastInfo& info,
    const std::string& reducer,
    const std::string& op,
    runtime::NDArray indptr, runtime::NDArray indices,
    runtime::NDArray rev_indptr, runtime::NDArray rev_indices,
    binary_op::Target lhs,
    binary_op::Target rhs,
    runtime::NDArray lhs_mapping,
    runtime::NDArray rhs_mapping,
    runtime::NDArray lhs_data,
    runtime::NDArray rhs_data,
    runtime::NDArray out_mapping,
    runtime::NDArray out_data);

template void BackwardBinaryReduceBcastImpl<kDLCPU>(
    const BcastInfo& info,
    const std::string& reducer,
    const std::string& op,
    runtime::NDArray indptr, runtime::NDArray indices,
    runtime::NDArray rev_indptr, runtime::NDArray rev_indices,
    binary_op::Target lhs_tgt, binary_op::Target rhs_tgt,
    runtime::NDArray lhs_mapping, runtime::NDArray rhs_mapping, runtime::NDArray out_mapping,
    runtime::NDArray lhs, runtime::NDArray rhs, runtime::NDArray out, runtime::NDArray grad_out,
    runtime::NDArray grad_lhs, runtime::NDArray grad_rhs);

}  // namespace kernel
}  // namespace dgl
