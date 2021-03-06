/*
 * The GURLS Package in C++
 *
 * Copyright (C) 2011-1013, IIT@MIT Lab
 * All rights reserved.
 *
 * authors:  M. Santoro
 * email:   msantoro@mit.edu
 * website: http://cbcl.mit.edu/IIT@MIT/IIT@MIT.html
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors or of the Massacusetts Institute of
 *       Technology or of the Italian Institute of Technology may be
 *       used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef _GURLS_CONFGAP_H_
#define _GURLS_CONFGAP_H_


#include "gurls++/confidence.h"
#include "gurls++/gmath.h"

namespace gurls {

/**
 * \ingroup Confidence
 * \brief ConfGap is the sub-class of Confidence that computes a confidence estimation for the predicted class.
 */

template <typename T>
class ConfGap: public Confidence<T>
{
public:
	///
	/// Default constructor
	///
	ConfGap():Confidence<T>("gap"){}
	
	///
	/// Clone method
	///
	TaskBase *clone()
	{
		return new ConfGap<T>();
	}

    /**
     * Computes a confidence estimation for the predicted class (i.e. highest scoring class). The difference between the highest scoring class and the second highest scoring class is used as an estimate.
     * \param X not used
     * \param Y not used
     * \param opt options with the following:
     *  - pred (settable with the class Prediction and its subclasses)
     *
     * \return ret, a GurlsOptionList with the following fields:
     *  - confidence = array containing the confidence score for each row of the field pred of opt.
	 *  - labels = array containing predicted class for each row of the field pred of opt.
     */
    GurlsOptionsList* execute(const gMat2D<T>& X, const gMat2D<T>& Y, const GurlsOptionsList& opt) throw(gException);
};

template<typename T>
GurlsOptionsList *ConfGap<T>::execute(const gMat2D<T>& /*X*/, const gMat2D<T>& /*Y*/, const GurlsOptionsList &opt) throw(gException)
{
    //   out = struct;
    //   [n,k] = size(opt.pred);
    const gMat2D<T> &pred = opt.getOptValue<OptMatrix<gMat2D<T> > >("pred");

    const unsigned long n = pred.rows();
    const unsigned long t = pred.cols();


//      out.confidence = opt.pred;
//      out.confidence = sort(out.confidence,2,'descend');
//      out.confidence = out.confidence(:,1) - out.confidence(:,2);

    const T* expscores = pred.getData();

    gMat2D<T> *conf = new gMat2D<T>(n,1);
    T* confidence = conf->getData();

    T* rowT = new T[t];

    for(unsigned long i=0; i<n; ++i)
    {
        getRow(expscores, n, t, i, rowT);
        std::sort(rowT,rowT+t);

        confidence[i] = rowT[t-1]-rowT[t-2];
    }

    delete [] rowT;

    GurlsOptionsList* ret = new GurlsOptionsList("confidence");
    ret->addOpt("confidence", new OptMatrix<gMat2D<T> >(*conf));

    return ret;
}

}

#endif //_GURLS_CONFGAP_H_
