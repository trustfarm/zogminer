/* MIT License
 *
 * Copyright (c) 2016 Omar Alvarez <omar.alvarez@udc.es>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __GPU_SOLVER_H
#define __GPU_SOLVER_H

#include <cstdio>
#include <csignal>
#include <iostream>

#include "crypto/equihash.h"
#include "cl_zogminer.h"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

// The maximum size of the .cl file we read in and compile
#define MAX_SOURCE_SIZE 	(0x200000)

#define EK 9
#define EN 200
#define DIGITBITS	(EN/(EK+1))

class GPUSolverCancelledException : public std::exception
{
    virtual const char* what() const throw() {
        return "GPU Equihash solver was cancelled";
    }
};

enum GPUSolverCancelCheck
{
    ListGenerationGPU,
    ListSortingGPU
};

class GPUSolver {

public:
	GPUSolver();
	~GPUSolver();
        bool run(unsigned int n, unsigned int k, const eh_HashState& base_state,
		            const std::function<bool(std::vector<unsigned char>)> validBlock,
				const std::function<bool(GPUSolverCancelCheck)> cancelled);

private:
	cl_zogminer * miner;
	bool GPU;
	bool initOK;
	static const uint32_t PROOFSIZE = 1 << EK;
	//TODO 20?
	uint32_t indices[20*PROOFSIZE];
	//Avg
	uint32_t counter = 0;
	float sum = 0.f;
	float avg = 0.f;

	bool GPUSolve200_9(const eh_HashState& base_state,
		         	const std::function<bool(std::vector<unsigned char>)> validBlock,
				const std::function<bool(GPUSolverCancelCheck)> cancelled);

};

#endif // __GPU_SOLVER_H
