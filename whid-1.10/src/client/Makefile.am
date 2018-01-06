
AUTOMAKE_OPTIONS=foreign
SUBDIRS=icons

bin_PROGRAMS = whid

whid_LDADD = -lwarcore -lwarice -lwarxml -lwardatabase -lwarwxwidgets \
	-lxerces-c -lxalan-c -lIce -lIceUtil \
	-L../libwhid -lwhid

INCLUDES = -I../../include -I../libwhid

CPPFLAGS = -DWHID_WITH_WXWIDGETS=1

whid_SOURCES= AboutDlg.cpp \
	app_resources.cpp \
	BillingWiz.cpp \
	ChooseRateDlg.cpp \
	CustomerDlg.cpp \
	EmailBillDlg.cpp \
	FilteredList.cpp \
	FirstUseDlg.cpp \
	ImageContainer.cpp \
	InvoiceUtil.cpp \
	LogonDlg.cpp \
	MainFrame.cpp \
	MysqlCreateDatabaseDlg.cpp \
	NewPasswdDlg.cpp \
	NodeDlg.cpp \
	NodeRateManager.cpp \
	ProjectDlg.cpp \
	PropertiesDlg.cpp \
	RateDlg.cpp \
	RateManager.cpp \
	SystemSettingsDlg.cpp \
	TaskDlg.cpp \
	TreeNode.cpp \
	TreeNodeCustomer.cpp \
	TreeNodeProject.cpp \
	TreeNodeTask.cpp \
	UserDlg.cpp \
	UserManager.cpp \
	WhidApp.cpp \
	WhidHtmlWindow.cpp \
	WorkDlg.cpp \
	WorkRateManager.cpp \
	HTMLReport.cpp


