
noinst_LIBRARIES=libwhid.a

INCLUDES= -I../../include

libwhid_a_SOURCES=CustomerI.cpp \
	GatewayI.cpp \
	HaveChargesI.cpp \
	HaveRatesI.cpp \
	InvoiceI.cpp \
	NodeI.cpp \
	ProjectI.cpp \
	RateI.cpp \
	TaskI.cpp \
	UserI.cpp \
	WhidControlI.cpp \
	WhidEngine.cpp \
	WorkI.cpp \
	whid.cpp

CustomerI.cpp \
	GatewayI.cpp \
	HaveChargesI.cpp \
	HaveRatesI.cpp \
	InvoiceI.cpp \
	NodeI.cpp \
	ProjectI.cpp \
	RateI.cpp \
	TaskI.cpp \
	UserI.cpp \
	WhidControlI.cpp \
	WhidEngine.cpp \
	WorkI.cpp: whid.h

whid.ice: whid.idl
	cp -f whid.idl whid.ice

whid.h whid.cpp: whid.ice
	rm -f whidI.cpp whidI.h
	slice2cpp --impl whid.ice

