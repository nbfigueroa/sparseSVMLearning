#====  files  =================================================================

MEXFILE := \
	gtsvm_mex.$(MEX_EXTENSION)

HEADERS := \
	../lib/gtsvm.h

LIBRARIES := \
	../lib/libgtsvm.a

LIBRARY_FLAGS := \
	-lcudart


#====  compilation rules  =====================================================

# disable default rules
.SUFFIXES:

.PHONY : all
all: $(MEXFILE)

%.$(MEX_EXTENSION) : %.c $(HEADERS) ${LIBRARIES}
	@echo "----  Building \"$@\" from \"$<\"  ----"
	$(MEX) $(MEXFLAGS) -I../lib -L../lib $< -lgtsvm $(LIBRARY_FLAGS)
	@echo

.PHONY : clean
clean :
	@echo "----  Cleaning  ----"
	rm -f $(MEXFILE)
	@echo
