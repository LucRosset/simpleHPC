CC		= /opt/openmpi-1.6.3/bin/mpicxx
OMPI_CXXFLAGS	= -I/opt/boost_1_48_0/include -I/opt/repasthpc-1.0.1/include -I/opt/netcdf-4.1.3/include -I/opt/hdf5-1.8.9/include -I/opt/openmpi-1.6.3/include
OMPI_LDFLAGS	= -L/opt/boost_1_48_0/lib -L/opt/repasthpc-1.0.1/lib -L/opt/netcdf-4.1.3/lib -L/opt/hdf5-1.8.9/lib -L/opt/openmpi-1.6.3/lib
OMPI_LIBS	= -lrelogo-1.0.1 -lrepast_hpc-1.0.1 -lboost_mpi -lboost_serialization -lboost_system -lboost_filesystem -lnetcdf -lnetcdf_c++ -lmpi -lmpi_cxx -ldl -lm -Wl,--export-dynamic -lrt -lnsl -lutil -lm -ldl

EXECF   = simple
EXEC	= bin/$(EXECF)

SRCDIR	= src
SRCS	= $(filter-out $(SRCDIR)/$(EXECF).cpp,$(wildcard $(SRCDIR)/*.cpp))
HEADERS	= $(wildcard $(SRCDIR)/*.h)

OBJDIR	= obj
OBJFS	= $(notdir $(SRCS:.cpp=.o))
OBJS	= $(addprefix $(OBJDIR)/,$(OBJFS))

Debug: $(EXEC)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(OMPI_CXXFLAGS) -c $< -o $@

$(EXEC): $(OBJS) $(HEADERS)
	$(CC) $(OMPI_CXXFLAGS) $(SRCDIR)/$(EXECF).cpp $(OMPI_LDFLAGS) $(OMPI_LIBS) $(OBJS) -o $(EXEC)

$(OBJS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)
