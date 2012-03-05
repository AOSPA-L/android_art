/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

namespace art {

/*
 * This file contains codegen for the MIPS32 ISA and is intended to be
 * includes by:
 *
 *        Codegen-$(TARGET_ARCH_VARIANT).c
 *
 */

static int coreRegs[] = {rAX, rCX, rDX, rBX, rSP, rBP, rSI, rDI}
static int reservedRegs[] = {rSP};
static int coreTemps[] = {rAX, rCX, rDX}
static int fpRegs[] = {fr0, fr1, fr2, fr3, fr4, fr5, fr6, fr7, fr8, fr9,
                       fr10, fr11, fr12, fr13, fr14, fr15}
static int fpTemps[] = {fr0, fr1, fr2, fr3, fr4, fr5, fr6, fr7, fr8, fr9,
                        fr10, fr11, fr12, fr13, fr14, fr15}

void genBarrier(CompilationUnit *cUnit);
void storePair(CompilationUnit *cUnit, int base, int lowReg,
               int highReg);
void loadPair(CompilationUnit *cUnit, int base, int lowReg, int highReg);
LIR *loadWordDisp(CompilationUnit *cUnit, int rBase, int displacement,
                      int rDest);
LIR *storeWordDisp(CompilationUnit *cUnit, int rBase,
                       int displacement, int rSrc);
LIR *loadConstant(CompilationUnit *cUnit, int rDest, int value);

LIR *fpRegCopy(CompilationUnit *cUnit, int rDest, int rSrc)
{
    UNIMPLEMENTED(WARNING) << "fpRegCopy";
    return NULL;
#if 0
    int opcode;
    /* must be both DOUBLE or both not DOUBLE */
    DCHECK_EQ(DOUBLEREG(rDest),DOUBLEREG(rSrc));
    if (DOUBLEREG(rDest)) {
        opcode = kMipsFmovd;
    } else {
        if (SINGLEREG(rDest)) {
            if (SINGLEREG(rSrc)) {
                opcode = kMipsFmovs;
            } else {
                /* note the operands are swapped for the mtc1 instr */
                int tOpnd = rSrc;
                rSrc = rDest;
                rDest = tOpnd;
                opcode = kMipsMtc1;
            }
        } else {
            DCHECK(SINGLEREG(rSrc));
            opcode = kMipsMfc1;
        }
    }
    LIR* res = rawLIR(cUnit, cUnit->currentDalvikOffset, opcode, rSrc, rDest);
    if (rDest == rSrc) {
        res->flags.isNop = true;
    }
    return res;
#endif
}

/*
 * Load a immediate using a shortcut if possible; otherwise
 * grab from the per-translation literal pool.  If target is
 * a high register, build constant into a low register and copy.
 *
 * No additional register clobbering operation performed. Use this version when
 * 1) rDest is freshly returned from oatAllocTemp or
 * 2) The codegen is under fixed register usage
 */
LIR *loadConstantNoClobber(CompilationUnit *cUnit, int rDest,
                               int value)
{
    UNIMPLEMENTED(WARNING) << "loadConstantNoClobber";
    return NULL;
#if 0
    LIR *res;

    int rDestSave = rDest;
    int isFpReg = FPREG(rDest);
    if (isFpReg) {
        DCHECK(SINGLEREG(rDest));
        rDest = oatAllocTemp(cUnit);
    }

    /* See if the value can be constructed cheaply */
    if (value == 0) {
        res = newLIR2(cUnit, kMipsMove, rDest, r_ZERO);
    } else if ((value > 0) && (value <= 65535)) {
        res = newLIR3(cUnit, kMipsOri, rDest, r_ZERO, value);
    } else if ((value < 0) && (value >= -32768)) {
        res = newLIR3(cUnit, kMipsAddiu, rDest, r_ZERO, value);
    } else {
        res = newLIR2(cUnit, kMipsLui, rDest, value>>16);
        if (value & 0xffff)
            newLIR3(cUnit, kMipsOri, rDest, rDest, value);
    }

    if (isFpReg) {
        newLIR2(cUnit, kMipsMtc1, rDest, rDestSave);
        oatFreeTemp(cUnit, rDest);
    }

    return res;
#endif
}

LIR *opNone(CompilationUnit *cUnit, OpKind op)
{
    UNIMPLEMENTED(WARNING) << "opNone";
    return NULL;
#if 0
    LIR *res;
    MipsOpCode opcode = kMipsNop;
    switch (op) {
        case kOpUncondBr:
            opcode = kMipsB;
            break;
        default:
            LOG(FATAL) << "Bad case in opNone";
    }
    res = newLIR0(cUnit, opcode);
    return res;
#endif
}

LIR *loadMultiple(CompilationUnit *cUnit, int rBase, int rMask);

LIR *opReg(CompilationUnit *cUnit, OpKind op, int rDestSrc)
{
    UNIMPLEMENTED(WARNING) << "opReg";
    return NULL;
#if 0
    MipsOpCode opcode = kMipsNop;
    switch (op) {
        case kOpBlx:
            opcode = kMipsJalr;
            break;
        case kOpBx:
            return newLIR1(cUnit, kMipsJr, rDestSrc);
            break;
        default:
            LOG(FATAL) << "Bad case in opReg";
    }
    return newLIR2(cUnit, opcode, r_RA, rDestSrc);
#endif
}

LIR *opRegRegImm(CompilationUnit *cUnit, OpKind op, int rDest,
                     int rSrc1, int value);
LIR *opRegImm(CompilationUnit *cUnit, OpKind op, int rDestSrc1,
                  int value)
{
    UNIMPLEMENTED(WARNING) << "opRegImm";
    return NULL;
#if 0
    LIR *res;
    bool neg = (value < 0);
    int absValue = (neg) ? -value : value;
    bool shortForm = (absValue & 0xff) == absValue;
    MipsOpCode opcode = kMipsNop;
    switch (op) {
        case kOpAdd:
            return opRegRegImm(cUnit, op, rDestSrc1, rDestSrc1, value);
            break;
        case kOpSub:
            return opRegRegImm(cUnit, op, rDestSrc1, rDestSrc1, value);
            break;
        default:
            LOG(FATAL) << "Bad case in opRegImm";
            break;
    }
    if (shortForm)
        res = newLIR2(cUnit, opcode, rDestSrc1, absValue);
    else {
        int rScratch = oatAllocTemp(cUnit);
        res = loadConstant(cUnit, rScratch, value);
        if (op == kOpCmp)
            newLIR2(cUnit, opcode, rDestSrc1, rScratch);
        else
            newLIR3(cUnit, opcode, rDestSrc1, rDestSrc1, rScratch);
    }
    return res;
#endif
}

LIR *opRegRegReg(CompilationUnit *cUnit, OpKind op, int rDest,
                           int rSrc1, int rSrc2)
{
    UNIMPLEMENTED(WARNING) << "opRegRegReg";
    return NULL;
#if 0
    MipsOpCode opcode = kMipsNop;
    switch (op) {
        case kOpAdd:
            opcode = kMipsAddu;
            break;
        case kOpSub:
            opcode = kMipsSubu;
            break;
        case kOpAnd:
            opcode = kMipsAnd;
            break;
        case kOpMul:
            opcode = kMipsMul;
            break;
        case kOpOr:
            opcode = kMipsOr;
            break;
        case kOpXor:
            opcode = kMipsXor;
            break;
        case kOpLsl:
            opcode = kMipsSllv;
            break;
        case kOpLsr:
            opcode = kMipsSrlv;
            break;
        case kOpAsr:
            opcode = kMipsSrav;
            break;
        case kOpAdc:
        case kOpSbc:
            LOG(FATAL) << "No carry bit on MIPS";
            break;
        default:
            LOG(FATAL) << "bad case in opRegRegReg";
            break;
    }
    return newLIR3(cUnit, opcode, rDest, rSrc1, rSrc2);
#endif
}

LIR *opRegRegImm(CompilationUnit *cUnit, OpKind op, int rDest,
                           int rSrc1, int value)
{
    UNIMPLEMENTED(WARNING) << "opRegRegImm";
    return NULL;
#if 0
    LIR *res;
    MipsOpCode opcode = kMipsNop;
    bool shortForm = true;

    switch(op) {
        case kOpAdd:
            if (IS_SIMM16(value)) {
                opcode = kMipsAddiu;
            }
            else {
                shortForm = false;
                opcode = kMipsAddu;
            }
            break;
        case kOpSub:
            if (IS_SIMM16((-value))) {
                value = -value;
                opcode = kMipsAddiu;
            }
            else {
                shortForm = false;
                opcode = kMipsSubu;
            }
            break;
        case kOpLsl:
                DCHECK(value >= 0 && value <= 31);
                opcode = kMipsSll;
                break;
        case kOpLsr:
                DCHECK(value >= 0 && value <= 31);
                opcode = kMipsSrl;
                break;
        case kOpAsr:
                DCHECK(value >= 0 && value <= 31);
                opcode = kMipsSra;
                break;
        case kOpAnd:
            if (IS_UIMM16((value))) {
                opcode = kMipsAndi;
            }
            else {
                shortForm = false;
                opcode = kMipsAnd;
            }
            break;
        case kOpOr:
            if (IS_UIMM16((value))) {
                opcode = kMipsOri;
            }
            else {
                shortForm = false;
                opcode = kMipsOr;
            }
            break;
        case kOpXor:
            if (IS_UIMM16((value))) {
                opcode = kMipsXori;
            }
            else {
                shortForm = false;
                opcode = kMipsXor;
            }
            break;
        case kOpMul:
            shortForm = false;
            opcode = kMipsMul;
            break;
        default:
            LOG(FATAL) << "Bad case in opRegRegImm";
            break;
    }

    if (shortForm)
        res = newLIR3(cUnit, opcode, rDest, rSrc1, value);
    else {
        if (rDest != rSrc1) {
            res = loadConstant(cUnit, rDest, value);
            newLIR3(cUnit, opcode, rDest, rSrc1, rDest);
        } else {
            int rScratch = oatAllocTemp(cUnit);
            res = loadConstant(cUnit, rScratch, value);
            newLIR3(cUnit, opcode, rDest, rSrc1, rScratch);
        }
    }
    return res;
#endif
}

LIR *opRegReg(CompilationUnit *cUnit, OpKind op, int rDestSrc1,
                  int rSrc2)
{
    UNIMPLEMENTED(WARNING) << "opRegReg";
    return NULL;
#if 0
    MipsOpCode opcode = kMipsNop;
    LIR *res;
    switch (op) {
        case kOpMov:
            opcode = kMipsMove;
            break;
        case kOpMvn:
            return newLIR3(cUnit, kMipsNor, rDestSrc1, rSrc2, r_ZERO);
        case kOpNeg:
            return newLIR3(cUnit, kMipsSubu, rDestSrc1, r_ZERO, rSrc2);
        case kOpAdd:
        case kOpAnd:
        case kOpMul:
        case kOpOr:
        case kOpSub:
        case kOpXor:
            return opRegRegReg(cUnit, op, rDestSrc1, rDestSrc1, rSrc2);
        case kOp2Byte:
#if __mips_isa_rev>=2
            res = newLIR2(cUnit, kMipsSeb, rDestSrc1, rSrc2);
#else
            res = opRegRegImm(cUnit, kOpLsl, rDestSrc1, rSrc2, 24);
            opRegRegImm(cUnit, kOpAsr, rDestSrc1, rDestSrc1, 24);
#endif
            return res;
        case kOp2Short:
#if __mips_isa_rev>=2
            res = newLIR2(cUnit, kMipsSeh, rDestSrc1, rSrc2);
#else
            res = opRegRegImm(cUnit, kOpLsl, rDestSrc1, rSrc2, 16);
            opRegRegImm(cUnit, kOpAsr, rDestSrc1, rDestSrc1, 16);
#endif
            return res;
        case kOp2Char:
             return newLIR3(cUnit, kMipsAndi, rDestSrc1, rSrc2, 0xFFFF);
        default:
            LOG(FATAL) << "Bad case in opRegReg";
            break;
    }
    return newLIR2(cUnit, opcode, rDestSrc1, rSrc2);
#endif
}

LIR *loadConstantValueWide(CompilationUnit *cUnit, int rDestLo,
                                     int rDestHi, int valLo, int valHi)
{
    LIR *res;
    res = loadConstantNoClobber(cUnit, rDestLo, valLo);
    loadConstantNoClobber(cUnit, rDestHi, valHi);
    return res;
}

/* Load value from base + scaled index. */
LIR *loadBaseIndexed(CompilationUnit *cUnit, int rBase,
                               int rIndex, int rDest, int scale, OpSize size)
{
    UNIMPLEMENTED(WARNING) << "loadBaseIndexed";
    return NULL;
#if 0
    LIR *first = NULL;
    LIR *res;
    MipsOpCode opcode = kMipsNop;
    int tReg = oatAllocTemp(cUnit);

    if (FPREG(rDest)) {
        DCHECK(SINGLEREG(rDest));
        DCHECK((size == kWord) || (size == kSingle));
        size = kSingle;
    } else {
        if (size == kSingle)
            size = kWord;
    }

    if (!scale) {
        first = newLIR3(cUnit, kMipsAddu, tReg , rBase, rIndex);
    } else {
        first = opRegRegImm(cUnit, kOpLsl, tReg, rIndex, scale);
        newLIR3(cUnit, kMipsAddu, tReg , rBase, tReg);
    }

    switch (size) {
        case kSingle:
            opcode = kMipsFlwc1;
            break;
        case kWord:
            opcode = kMipsLw;
            break;
        case kUnsignedHalf:
            opcode = kMipsLhu;
            break;
        case kSignedHalf:
            opcode = kMipsLh;
            break;
        case kUnsignedByte:
            opcode = kMipsLbu;
            break;
        case kSignedByte:
            opcode = kMipsLb;
            break;
        default:
            LOG(FATAL) << "Bad case in loadBaseIndexed";
    }

    res = newLIR3(cUnit, opcode, rDest, 0, tReg);
    oatFreeTemp(cUnit, tReg);
    return (first) ? first : res;
#endif
}

/* store value base base + scaled index. */
LIR *storeBaseIndexed(CompilationUnit *cUnit, int rBase,
                                int rIndex, int rSrc, int scale, OpSize size)
{
    UNIMPLEMENTED(WARNING) << "storeBaseIndexed";
    return NULL;
#if 0
    LIR *first = NULL;
    LIR *res;
    MipsOpCode opcode = kMipsNop;
    int rNewIndex = rIndex;
    int tReg = oatAllocTemp(cUnit);

#ifdef __mips_hard_float
    if (FPREG(rSrc)) {
        DCHECK(SINGLEREG(rSrc));
        DCHECK((size == kWord) || (size == kSingle));
        size = kSingle;
    } else {
        if (size == kSingle)
            size = kWord;
    }
#endif

    if (!scale) {
        first = newLIR3(cUnit, kMipsAddu, tReg , rBase, rIndex);
    } else {
        first = opRegRegImm(cUnit, kOpLsl, tReg, rIndex, scale);
        newLIR3(cUnit, kMipsAddu, tReg , rBase, tReg);
    }

    switch (size) {
#ifdef __mips_hard_float
        case kSingle:
            opcode = kMipsFswc1;
            break;
#endif
        case kWord:
            opcode = kMipsSw;
            break;
        case kUnsignedHalf:
        case kSignedHalf:
            opcode = kMipsSh;
            break;
        case kUnsignedByte:
        case kSignedByte:
            opcode = kMipsSb;
            break;
        default:
            LOG(FATAL) << "Bad case in storeBaseIndexed";
    }
    res = newLIR3(cUnit, opcode, rSrc, 0, tReg);
    oatFreeTemp(cUnit, rNewIndex);
    return first;
#endif
}

LIR *loadMultiple(CompilationUnit *cUnit, int rBase, int rMask)
{
    UNIMPLEMENTED(WARNING) << "loadMultiple";
    return NULL;
#if 0
    int i;
    int loadCnt = 0;
    LIR *res = NULL ;
    genBarrier(cUnit);

    for (i = 0; i < 8; i++, rMask >>= 1) {
        if (rMask & 0x1) { /* map r0 to MIPS r_A0 */
            newLIR3(cUnit, kMipsLw, i+r_A0, loadCnt*4, rBase);
            loadCnt++;
        }
    }

    if (loadCnt) {/* increment after */
        newLIR3(cUnit, kMipsAddiu, rBase, rBase, loadCnt*4);
    }

    genBarrier(cUnit);
    return res; /* NULL always returned which should be ok since no callers use it */
#endif
}

LIR *storeMultiple(CompilationUnit *cUnit, int rBase, int rMask)
{
    UNIMPLEMENTED(WARNING) << "storeMultiple";
    return NULL;
    int i;
    int storeCnt = 0;
    LIR *res = NULL ;
    genBarrier(cUnit);

    for (i = 0; i < 8; i++, rMask >>= 1) {
        if (rMask & 0x1) { /* map r0 to MIPS r_A0 */
            newLIR3(cUnit, kMipsSw, i+r_A0, storeCnt*4, rBase);
            storeCnt++;
        }
    }

    if (storeCnt) { /* increment after */
        newLIR3(cUnit, kMipsAddiu, rBase, rBase, storeCnt*4);
    }

    genBarrier(cUnit);
    return res; /* NULL always returned which should be ok since no callers use it */
#endif
}

LIR *loadBaseDispBody(CompilationUnit *cUnit, MIR *mir, int rBase,
                                int displacement, int rDest, int rDestHi,
                                OpSize size, int sReg)
/*
 * Load value from base + displacement.  Optionally perform null check
 * on base (which must have an associated sReg and MIR).  If not
 * performing null check, incoming MIR can be null. IMPORTANT: this
 * code must not allocate any new temps.  If a new register is needed
 * and base and dest are the same, spill some other register to
 * rlp and then restore.
 */
{
    UNIMPLEMENTED(WARNING) << "loadBaseDispBody";
    return NULL;
#if 0
    LIR *res;
    LIR *load = NULL;
    LIR *load2 = NULL;
    MipsOpCode opcode = kMipsNop;
    bool shortForm = IS_SIMM16(displacement);
    bool pair = false;

    switch (size) {
        case kLong:
        case kDouble:
            pair = true;
            opcode = kMipsLw;
            if (FPREG(rDest)) {
                opcode = kMipsFlwc1;
                if (DOUBLEREG(rDest)) {
                    rDest = rDest - FP_DOUBLE;
                } else {
                    DCHECK(FPREG(rDestHi));
                    DCHECK(rDest == (rDestHi - 1));
                }
                rDestHi = rDest + 1;
            }
            shortForm = IS_SIMM16_2WORD(displacement);
            DCHECK_EQ((displacement & 0x3), 0);
            break;
        case kWord:
        case kSingle:
            opcode = kMipsLw;
            if (FPREG(rDest)) {
                opcode = kMipsFlwc1;
                DCHECK(SINGLEREG(rDest));
            }
            DCHECK_EQ((displacement & 0x3), 0);
            break;
        case kUnsignedHalf:
            opcode = kMipsLhu;
            DCHECK_EQ((displacement & 0x1), 0);
            break;
        case kSignedHalf:
            opcode = kMipsLh;
            DCHECK_EQ((displacement & 0x1), 0);
            break;
        case kUnsignedByte:
            opcode = kMipsLbu;
            break;
        case kSignedByte:
            opcode = kMipsLb;
            break;
        default:
            LOG(FATAL) << "Bad case in loadBaseIndexedBody";
    }

    if (shortForm) {
        if (!pair) {
            load = res = newLIR3(cUnit, opcode, rDest, displacement, rBase);
        } else {
            load = res = newLIR3(cUnit, opcode, rDest, displacement + LOWORD_OFFSET, rBase);
            load2 = newLIR3(cUnit, opcode, rDestHi, displacement + HIWORD_OFFSET, rBase);
        }
    } else {
        if (pair) {
            int rTmp = oatAllocFreeTemp(cUnit);
            res = opRegRegImm(cUnit, kOpAdd, rTmp, rBase, displacement);
            load = newLIR3(cUnit, opcode, rDest, LOWORD_OFFSET, rTmp);
            load2 = newLIR3(cUnit, opcode, rDestHi, HIWORD_OFFSET, rTmp);
            oatFreeTemp(cUnit, rTmp);
        } else {
            int rTmp = (rBase == rDest) ? oatAllocFreeTemp(cUnit)
                                        : rDest;
            res = loadConstant(cUnit, rTmp, displacement);
            load = newLIR3(cUnit, opcode, rDest, rBase, rTmp);
            if (rTmp != rDest)
                oatFreeTemp(cUnit, rTmp);
        }
    }

    if (rBase == rSP) {
        if (load != NULL)
            annotateDalvikRegAccess(load, (displacement + (pair ? LOWORD_OFFSET : 0)) >> 2,
                                    true /* isLoad */);
        if (load2 != NULL)
            annotateDalvikRegAccess(load2, (displacement + HIWORD_OFFSET) >> 2,
                                    true /* isLoad */);
    }
    return load;
#endif
}

LIR *loadBaseDisp(CompilationUnit *cUnit, MIR *mir, int rBase,
                            int displacement, int rDest, OpSize size,
                            int sReg)
{
    return loadBaseDispBody(cUnit, mir, rBase, displacement, rDest, -1,
                            size, sReg);
}

LIR *loadBaseDispWide(CompilationUnit *cUnit, MIR *mir, int rBase,
                          int displacement, int rDestLo, int rDestHi,
                                int sReg)
{
    return loadBaseDispBody(cUnit, mir, rBase, displacement, rDestLo, rDestHi,
                            kLong, sReg);
}

LIR *storeBaseDispBody(CompilationUnit *cUnit, int rBase,
                                 int displacement, int rSrc, int rSrcHi,
                                 OpSize size)
{
    UNIMPLEMENTED(WARNING) << "storeBaseDispBody";
    return NULL;
#if 0
    LIR *res;
    LIR *store = NULL;
    LIR *store2 = NULL;
    MipsOpCode opcode = kMipsNop;
    bool shortForm = IS_SIMM16(displacement);
    bool pair = false;

    switch (size) {
        case kLong:
        case kDouble:
            pair = true;
            opcode = kMipsSw;
#ifdef __mips_hard_float
            if (FPREG(rSrc)) {
                opcode = kMipsFswc1;
                if (DOUBLEREG(rSrc)) {
                    rSrc = rSrc - FP_DOUBLE;
                } else {
                    DCHECK(FPREG(rSrcHi));
                    DCHECK_EQ(rSrc, (rSrcHi - 1));
                }
                rSrcHi = rSrc + 1;
            }
#endif
            shortForm = IS_SIMM16_2WORD(displacement);
            DCHECK_EQ((displacement & 0x3), 0);
            break;
        case kWord:
        case kSingle:
            opcode = kMipsSw;
#ifdef __mips_hard_float
            if (FPREG(rSrc)) {
                opcode = kMipsFswc1;
                DCHECK(SINGLEREG(rSrc));
            }
#endif
            DCHECK_EQ((displacement & 0x3), 0);
            break;
        case kUnsignedHalf:
        case kSignedHalf:
            opcode = kMipsSh;
            DCHECK_EQ((displacement & 0x1), 0);
            break;
        case kUnsignedByte:
        case kSignedByte:
            opcode = kMipsSb;
            break;
        default:
            LOG(FATAL) << "Bad case in storeBaseIndexedBody";
    }

    if (shortForm) {
        if (!pair) {
            store = res = newLIR3(cUnit, opcode, rSrc, displacement, rBase);
        } else {
            store = res = newLIR3(cUnit, opcode, rSrc, displacement + LOWORD_OFFSET, rBase);
            store2 = newLIR3(cUnit, opcode, rSrcHi, displacement + HIWORD_OFFSET, rBase);
        }
    } else {
        int rScratch = oatAllocTemp(cUnit);
        res = opRegRegImm(cUnit, kOpAdd, rScratch, rBase, displacement);
        if (!pair) {
            store =  newLIR3(cUnit, opcode, rSrc, 0, rScratch);
        } else {
            store =  newLIR3(cUnit, opcode, rSrc, LOWORD_OFFSET, rScratch);
            store2 = newLIR3(cUnit, opcode, rSrcHi, HIWORD_OFFSET, rScratch);
        }
        oatFreeTemp(cUnit, rScratch);
    }

    if (rBase == rSP) {
        if (store != NULL)
            annotateDalvikRegAccess(store, (displacement + (pair ? LOWORD_OFFSET : 0)) >> 2,
                                    false /* isLoad */);
        if (store2 != NULL)
            annotateDalvikRegAccess(store2, (displacement + HIWORD_OFFSET) >> 2,
                                    false /* isLoad */);
    }

    return res;
#endif
}

LIR *storeBaseDisp(CompilationUnit *cUnit, int rBase,
                       int displacement, int rSrc, OpSize size)
{
    return storeBaseDispBody(cUnit, rBase, displacement, rSrc, -1, size);
}

LIR *storeBaseDispWide(CompilationUnit *cUnit, int rBase,
                           int displacement, int rSrcLo, int rSrcHi)
{
    return storeBaseDispBody(cUnit, rBase, displacement, rSrcLo, rSrcHi, kLong);
}

void storePair(CompilationUnit *cUnit, int base, int lowReg, int highReg)
{
    storeWordDisp(cUnit, base, LOWORD_OFFSET, lowReg);
    storeWordDisp(cUnit, base, HIWORD_OFFSET, highReg);
}

void loadPair(CompilationUnit *cUnit, int base, int lowReg, int highReg)
{
    loadWordDisp(cUnit, base, LOWORD_OFFSET , lowReg);
    loadWordDisp(cUnit, base, HIWORD_OFFSET , highReg);
}

}  // namespace art
