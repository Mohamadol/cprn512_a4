VectorBlox MXP
================

The VectorBlox MXP Matrix Processor is an FPGA-based soft processor
capable of highly parallel execution. Programmed entirely in C/C++,
the MXP is capable of executing data-parallel software algorithms at
hardware-like speeds. MXP's parameterrized design lets the user specify
the amount of parallelism required, ranging from 1 to 128 or more
parallel ALUs. Key features of the MXP include a parallel-access
scratchpad memory to hold vector data and a high-throughput DMA
engine.

VectorBlox Computing licenses the MXP processor as a synthesizable IP
core for use in FPGA designs.

This preview release is for evaluation only, and consists of:

- Pre-built bitstreams to evaluate the MXP processor on several popular
  Altera and Xilinx FPGA development boards
- The MXP Software Development Kit (SDK)
- Example programs
- Documentation

To get started immediately:

- Ensure you have one of the [supported development
  boards](http://vectorblox.github.io/mxp/mxp_supported_boards.html).

- Download the latest release zip file
  [Full](https://github.com/VectorBlox/mxp/archive/master.zip) /
  [Simulator Only](https://github.com/VectorBlox/mxp/archive/simulator_only.zip)).

- Follow the VectorBlox MXP Hardware & Software Quickstart for
  [Quartus](http://vectorblox.github.io/mxp/mxp_quickstart_altera.html) /
  [Vivado](http://vectorblox.github.io/mxp/mxp_quickstart_vivado.html) /
  [Simulator](http://vectorblox.github.io/mxp/mxp_quickstart_simulator.html)

For further VectorBlox MXP documentation, refer to the following:

- VectorBlox MXP Programming Guide for [Altera](http://vectorblox.github.io/mxp/mxp_guide_altera.html) /[Xilinx](http://vectorblox.github.io/mxp/mxp_guide_xilinx.html)

- [VectorBlox MXP Programming  Reference](http://vectorblox.github.io/mxp/mxp_reference.html)

### API Changes March 2018
There are several breaking changes to the API since between the 2017 and 2018
releases. Below is an overview of the changes, please see the Programming Reference
(link above) for more details on the current API.

* Vector Shifts are srca >> srcb rather than srcb>> srca.
This leads to some issues with shifting by scalar values which can be resolved
by mapping shifting by scalar values to VMUL, and VMULH for shifting left
and right respectively.

* All vector instructions are 3 dimensional, to do 1 or 2 dimensional instructions
set nmats and/or nrows to 1. Because of this the `vbx_set_vl`,`vbx_set_2D`, and
`vbx_set_3D` have changed. They now all accept 3 arguments.

* Destination, source A and source B all have independant sizes and signedness.

### Contact information
For further information please email us at
[info@vectorblox.com](mailto:info@vectorblox.com) or visit
<http://www.vectorblox.com>.
