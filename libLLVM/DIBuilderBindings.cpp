//===- DIBuilderBindings.cpp - Bindings for DIBuilder ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines C bindings for the DIBuilder class.
//
//===----------------------------------------------------------------------===//

#include "DIBuilderBindings.h"
#include "llvm/IR/DIBuilder.h"
#include "llvm/IR/Module.h"

using namespace llvm;
using namespace llvm::dwarf;

template <typename DIT> DIT *unwrapDI(LLVMMetadataRef Ref) {
	return (DIT *)(Ref ? unwrap<MDNode>(Ref) : nullptr);
}

LLVMDIBuilderRef LLVMNewDIBuilder(LLVMModuleRef mref) {
    Module *m = unwrap(mref);
    return llvm::wrap(new DIBuilder(*m));
}

void LLVMDIBuilderDestroy(LLVMDIBuilderRef dref) {
    DIBuilder *d = llvm::unwrap(dref);
    delete d;
}

LLVMMetadataRef LLVMDIBuilderCreateLexicalBlock(LLVMDIBuilderRef Dref,
    LLVMMetadataRef Scope,
    LLVMMetadataRef File,
    unsigned Line,
    unsigned Column) {
    DIBuilder *D = unwrap(Dref);
    auto *LB = D->createLexicalBlock(unwrap<DILocalScope>(Scope),
        unwrap<DIFile>(File), Line, Column);
    return wrap(LB);
}

LLVMMetadataRef LLVMDIBuilderCreateLexicalBlockFile(LLVMDIBuilderRef Dref,
    LLVMMetadataRef Scope,
    LLVMMetadataRef File,
    unsigned Discriminator) {
    DIBuilder *D = unwrap(Dref);
    return wrap(D->createLexicalBlockFile(unwrap<DILocalScope>(Scope),
        unwrap<DIFile>(File), Discriminator));
}

LLVMMetadataRef LLVMDIBuilderCreateFunction(
    LLVMDIBuilderRef Dref, LLVMMetadataRef Scope, const char *Name,
    const char *LinkageName, LLVMMetadataRef File, unsigned Line,
    LLVMMetadataRef CompositeType, int IsLocalToUnit, int IsDefinition,
    unsigned ScopeLine, unsigned Flags, int IsOptimized) {
    DIBuilder *D = unwrap(Dref);
    return wrap(D->createFunction(unwrap<DIScope>(Scope), Name, LinkageName,
        File ? unwrap<DIFile>(File) : nullptr, Line,
        unwrap<DISubroutineType>(CompositeType),
        IsLocalToUnit, IsDefinition, ScopeLine, (DINode::DIFlags)Flags,
        IsOptimized));
}

LLVMMetadataRef
LLVMDIBuilderCreateAutoVariable(LLVMDIBuilderRef Dref, LLVMMetadataRef Scope,
    const char *Name, LLVMMetadataRef File,
    unsigned Line, LLVMMetadataRef Ty,
    int AlwaysPreserve, unsigned Flags) {
    DIBuilder *D = unwrap(Dref);
    return wrap(D->createAutoVariable(unwrap<DIScope>(Scope), Name,
        unwrap<DIFile>(File), Line,
        unwrap<DIType>(Ty), AlwaysPreserve, (DINode::DIFlags)Flags));
}

LLVMMetadataRef LLVMDIBuilderCreateParameterVariable(
    LLVMDIBuilderRef Dref, LLVMMetadataRef Scope, const char *Name,
    unsigned ArgNo, LLVMMetadataRef File, unsigned Line, LLVMMetadataRef Ty,
    int AlwaysPreserve, unsigned Flags) {
    DIBuilder *D = unwrap(Dref);
    return wrap(D->createParameterVariable(
        unwrap<DIScope>(Scope), Name, ArgNo, unwrap<DIFile>(File), Line,
        unwrap<DIType>(Ty), AlwaysPreserve, (DINode::DIFlags)Flags));
}

LLVMMetadataRef
LLVMDIBuilderCreateBasicType(LLVMDIBuilderRef Builder, const char *Name,
	size_t NameLen, uint64_t SizeInBits,
	unsigned Encoding) {
	return llvm::wrap(unwrap(Builder)->createBasicType({ Name, NameLen }, SizeInBits, Encoding));
}

LLVMMetadataRef LLVMDIBuilderCreatePointerType(
	LLVMDIBuilderRef Builder, LLVMMetadataRef PointeeTy,
	uint64_t SizeInBits, uint32_t AlignInBits, unsigned AddressSpace,
	const char *Name, size_t NameLen) {

	return wrap(unwrap(Builder)->createPointerType(unwrapDI<DIType>(PointeeTy),
		SizeInBits, AlignInBits,
		AddressSpace, { Name, NameLen }));
}

LLVMMetadataRef
LLVMDIBuilderCreateSubroutineType(LLVMDIBuilderRef Dref, LLVMMetadataRef File,
    LLVMMetadataRef ParameterTypes) {
    DIBuilder *D = unwrap(Dref);
    return wrap(
        D->createSubroutineType(DITypeRefArray(unwrap<MDTuple>(ParameterTypes))));
}

LLVMMetadataRef LLVMDIBuilderCreateStructType(
    LLVMDIBuilderRef Dref, LLVMMetadataRef Scope, const char *Name,
    LLVMMetadataRef File, unsigned Line, uint64_t SizeInBits,
    uint32_t AlignInBits, unsigned Flags, LLVMMetadataRef DerivedFrom,
    LLVMMetadataRef ElementTypes) {
    DIBuilder *D = unwrap(Dref);
    return wrap(D->createStructType(
        unwrap<DIScope>(Scope), Name, File ? unwrap<DIFile>(File) : nullptr, Line,
        SizeInBits, AlignInBits, (DINode::DIFlags)Flags,
        DerivedFrom ? unwrap<DIType>(DerivedFrom) : nullptr,
        ElementTypes ? DINodeArray(unwrap<MDTuple>(ElementTypes)) : nullptr));
}

LLVMMetadataRef LLVMDIBuilderCreateReplaceableCompositeType(
    LLVMDIBuilderRef Dref, unsigned Tag, const char *Name,
    LLVMMetadataRef Scope, LLVMMetadataRef File, unsigned Line,
    unsigned RuntimeLang, uint64_t SizeInBits, uint32_t AlignInBits,
    unsigned Flags) {
    DIBuilder *D = unwrap(Dref);
    return wrap(D->createReplaceableCompositeType(
        Tag, Name, unwrap<DIScope>(Scope), File ? unwrap<DIFile>(File) : nullptr,
        Line, RuntimeLang, SizeInBits, AlignInBits, (DINode::DIFlags)Flags));
}

LLVMMetadataRef
LLVMDIBuilderCreateMemberType(LLVMDIBuilderRef Dref, LLVMMetadataRef Scope,
    const char *Name, LLVMMetadataRef File,
    unsigned Line, uint64_t SizeInBits,
    uint32_t AlignInBits, uint64_t OffsetInBits,
    unsigned Flags, LLVMMetadataRef Ty) {
    DIBuilder *D = unwrap(Dref);
    return wrap(D->createMemberType(
        unwrap<DIScope>(Scope), Name, File ? unwrap<DIFile>(File) : nullptr, Line,
        SizeInBits, AlignInBits, OffsetInBits, (DINode::DIFlags)Flags, unwrap<DIType>(Ty)));
}

LLVMMetadataRef LLVMDIBuilderCreateArrayType(LLVMDIBuilderRef Dref,
    uint64_t SizeInBits,
    uint32_t AlignInBits,
    LLVMMetadataRef ElementType,
    LLVMMetadataRef Subscripts) {
    DIBuilder *D = unwrap(Dref);
    return wrap(D->createArrayType(SizeInBits, AlignInBits,
        unwrap<DIType>(ElementType),
        DINodeArray(unwrap<MDTuple>(Subscripts))));
}

LLVMMetadataRef LLVMDIBuilderCreateTypedef(LLVMDIBuilderRef Dref,
    LLVMMetadataRef Ty, const char *Name,
    LLVMMetadataRef File, unsigned Line,
    LLVMMetadataRef Context) {
    DIBuilder *D = unwrap(Dref);
    return wrap(D->createTypedef(unwrap<DIType>(Ty), Name,
        File ? unwrap<DIFile>(File) : nullptr, Line,
        Context ? unwrap<DIScope>(Context) : nullptr));
}

LLVMMetadataRef LLVMDIBuilderGetOrCreateSubrange(LLVMDIBuilderRef Dref,
    int64_t Lo, int64_t Count) {
    DIBuilder *D = unwrap(Dref);
    return wrap(D->getOrCreateSubrange(Lo, Count));
}

LLVMMetadataRef LLVMDIBuilderGetOrCreateArray(LLVMDIBuilderRef Dref,
    LLVMMetadataRef *Data,
    size_t Length) {
    DIBuilder *D = unwrap(Dref);
    Metadata **DataValue = unwrap(Data);
    ArrayRef<Metadata *> Elements(DataValue, Length);
    DINodeArray A = D->getOrCreateArray(Elements);
    return wrap(A.get());
}

LLVMMetadataRef LLVMDIBuilderGetOrCreateTypeArray(LLVMDIBuilderRef Dref,
    LLVMMetadataRef *Data,
    size_t Length) {
    DIBuilder *D = unwrap(Dref);
    Metadata **DataValue = unwrap(Data);
    ArrayRef<Metadata *> Elements(DataValue, Length);
    DITypeRefArray A = D->getOrCreateTypeArray(Elements);
    return wrap(A.get());
}

LLVMMetadataRef LLVMDIBuilderCreateExpression(LLVMDIBuilderRef Dref,
    int64_t *Addr, size_t Length) {
    DIBuilder *D = unwrap(Dref);
    return wrap(D->createExpression(ArrayRef<int64_t>(Addr, Length)));
}