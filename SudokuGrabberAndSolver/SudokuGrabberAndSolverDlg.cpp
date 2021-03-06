
// SudokuGrabberAndSolverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SudokuGrabberAndSolver.h"
#include "SudokuGrabberAndSolverDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSudokuGrabberAndSolverDlg dialog



CSudokuGrabberAndSolverDlg::CSudokuGrabberAndSolverDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SUDOKUGRABBERANDSOLVER_DIALOG, pParent)
	,m_pImg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			m_nValues[i][j] = 0;
	ClearPossibleAnswer();
}

void CSudokuGrabberAndSolverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOGEDIT, m_LogBox);
	DDX_Control(pDX, IDC_DISPLAYWINDOW, m_Panel);
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			DDX_Text(pDX, IDC_EDIT11 + j + i * 10, m_nValues[i][j]);
			DDV_MinMaxInt(pDX, m_nValues[i][j], 0, 9);
		}
	}
}

BEGIN_MESSAGE_MAP(CSudokuGrabberAndSolverDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDTRAINING, &CSudokuGrabberAndSolverDlg::OnBnClickedTraining)
	ON_BN_CLICKED(IDTESTTRAINING, &CSudokuGrabberAndSolverDlg::OnBnClickedTesttraining)
	ON_BN_CLICKED(IDREADSUDOKU, &CSudokuGrabberAndSolverDlg::OnBnClickedReadsudoku)
	ON_BN_CLICKED(IDSOLVESUDOKU, &CSudokuGrabberAndSolverDlg::OnBnClickedSolvesudoku)
	ON_BN_CLICKED(IDEXIT, &CSudokuGrabberAndSolverDlg::OnBnClickedExit)
END_MESSAGE_MAP()


// CSudokuGrabberAndSolverDlg message handlers

BOOL CSudokuGrabberAndSolverDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_Panel.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSudokuGrabberAndSolverDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSudokuGrabberAndSolverDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSudokuGrabberAndSolverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSudokuGrabberAndSolverDlg::LogStatus(char*  text, ...)
{

	char buf[512] = { 0, };
	va_list ap;

	va_start(ap, text);

	//vswprintf(buf + wcslen(buf), fmt, ap);
	vsprintf_s(buf, 512, text, ap);
	va_end(ap);



	const size_t cSize = strlen(buf) + 1;
	wchar_t* wc = new wchar_t[cSize];
	size_t outSize;

	mbstowcs_s(&outSize, wc, cSize, buf, cSize - 1);

	int nLength = m_LogBox.GetWindowTextLength();
	m_LogBox.SetSel(nLength, nLength);
	m_LogBox.ReplaceSel(wc);
	delete[]wc;
}

void	CSudokuGrabberAndSolverDlg::DrawOnPanel(int OriginXsize, int OriginYSize, cv::Mat dataGraphic) 
{
	OriginXsize = OriginXsize / 4 * 4;

	int xSize = OriginXsize < 500 ? OriginXsize : 500;
	int ySize = OriginYSize < 350 ? OriginYSize : 350;

	m_Panel.MoveWindow(10, 10, xSize, ySize);
	m_Panel.ShowWindow(SW_SHOW);
	if (m_pImg)
	{
		m_pImg->ReleaseDC();
		delete m_pImg;
		m_pImg = NULL;
	}
	m_pImg = new CImage;
	m_pImg->Create(xSize, ySize, 24);
	cv::Mat displayTraining;			// for display on the dlg.
	cv::resize(dataGraphic, displayTraining, cv::Size(xSize, ySize));
	cv::flip(displayTraining, displayTraining, 0);
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 24;
	bitInfo.bmiHeader.biWidth = xSize;
	bitInfo.bmiHeader.biHeight = ySize;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant =
		bitInfo.bmiHeader.biClrUsed =
		bitInfo.bmiHeader.biSizeImage =
		bitInfo.bmiHeader.biXPelsPerMeter =
		bitInfo.bmiHeader.biYPelsPerMeter = 0;

	StretchDIBits(m_pImg->GetDC(), 0, 0,
		xSize, ySize, 0, 0,
		xSize, ySize,
		displayTraining.data, &bitInfo, DIB_RGB_COLORS, SRCCOPY);

	m_pImg->BitBlt(::GetDC(m_Panel.m_hWnd), 0, 0);
	displayTraining.release();
}

void CSudokuGrabberAndSolverDlg::OnBnClickedTraining()
{
	char strFilter[] = { "All Files (*.*)|*.*|" };
	CFileDialog FileDlg(TRUE, CString("Open Graphic file"), NULL, 0, CString(strFilter));
	CString filename;
	if (FileDlg.DoModal() == IDOK)
	{
		filename = FileDlg.GetPathName(); // return full path and filename
	}
	else
		return;
	USES_CONVERSION;
	const char* strFilename = T2A((LPCTSTR)filename);
	/////////////////////////////////////////////////
	//check previous file//
	cv::Mat matClassificationInts;
	cv::FileStorage fsClassifications("classifications.xml", cv::FileStorage::READ);
	if (fsClassifications.isOpened() == false)
	{
		LogStatus("error, unable to open training classifications file, exiting program\n");
		LogStatus("Make file from the scratch\n");
	}
	else
	{
		fsClassifications["classifications"] >> matClassificationInts;
		fsClassifications.release();
	}
	cv::Mat matTrainingImagesAsFlattenedFloats;
	cv::FileStorage fsTrainingImages("images.xml", cv::FileStorage::READ);
	if (fsTrainingImages.isOpened() == false) {
		LogStatus("error, unable to open training images file, exiting program\n");
		LogStatus("Make file from the scratch\n");
	}
	else
	{
		fsTrainingImages["images"] >> matTrainingImagesAsFlattenedFloats;
		fsTrainingImages.release();
	}
	//////////////////////////////////////////////////////

	cv::Mat imgTrainingNumbers;
	cv::Mat imgGrayscale;
	cv::Mat imgBlurred;
	cv::Mat imgThresh;
	cv::Mat imgThreshCopy;
	std::vector<std::vector<cv::Point> > ptContours;
	std::vector<cv::Vec4i> v4iHierarchy;

	std::vector<int> intValidChars = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', };

	imgTrainingNumbers = cv::imread(strFilename);

	if (imgTrainingNumbers.empty()) {
		LogStatus("error: image not read from file\n");
		return;
	}
	cv::cvtColor(imgTrainingNumbers, imgGrayscale, CV_BGR2GRAY);	// make grayscale
	cv::GaussianBlur(imgGrayscale, imgBlurred, cv::Size(3, 3), 0);	//gaussian blue
	cv::adaptiveThreshold(imgBlurred, imgThresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 7, 2);	// binary.

	imgThreshCopy = imgThresh.clone();
	cv::findContours(imgThreshCopy, ptContours, v4iHierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < ptContours.size(); i++) {
		if (cv::contourArea(ptContours[i]) > MIN_CONTOUR_AREA) {
			cv::Rect boundingRect = cv::boundingRect(ptContours[i]);

			cv::rectangle(imgTrainingNumbers, boundingRect, cv::Scalar(0, 0, 255), 2);
			cv::Mat matROI = imgThresh(boundingRect);
			cv::Mat matROIResized;
			cv::resize(matROI, matROIResized, cv::Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));

			cv::imshow("matROIResized", matROIResized);
			
			DrawOnPanel(imgTrainingNumbers.size().width, imgTrainingNumbers.size().height,imgTrainingNumbers);

			int intChar = cv::waitKey(0);

			if (intChar == 27) {
				return;
			}
			else if (std::find(intValidChars.begin(), intValidChars.end(), intChar) != intValidChars.end()) {
				matClassificationInts.push_back(intChar);
				cv::Mat matImageFloat;
				matROIResized.convertTo(matImageFloat, CV_32FC1);
				cv::Mat matImageFlattenedFloat = matImageFloat.reshape(1, 1);
				matTrainingImagesAsFlattenedFloats.push_back(matImageFlattenedFloat);
				LogStatus("%d is learned\n", intChar);
			}
		}
	}

	LogStatus("training complete\n");
	// save learning data to xml file
	cv::FileStorage fsClassifications1("classifications.xml", cv::FileStorage::WRITE);
	if (fsClassifications1.isOpened() == false) {
		LogStatus("error, unable to open training classifications file, exiting program\n");
		return;
	}
	fsClassifications1 << "classifications" << matClassificationInts;
	fsClassifications1.release();
	cv::FileStorage fsTrainingImages1("images.xml", cv::FileStorage::WRITE);
	if (fsTrainingImages1.isOpened() == false) {
		LogStatus("error, unable to open training images file, exiting program\n");
		return;
	}
	fsTrainingImages1 << "images" << matTrainingImagesAsFlattenedFloats;
	fsTrainingImages1.release();
	LogStatus("Learning data is made\n");

	imgTrainingNumbers.release();
	imgGrayscale.release();
	imgBlurred.release();
	imgThresh.release();
	imgThreshCopy.release();

	cv::destroyAllWindows();
	return;
}
void CSudokuGrabberAndSolverDlg::OnBnClickedTesttraining()
{
	char strFilter[] = { "All Files (*.*)|*.*|" };
	CFileDialog FileDlg(TRUE, CString("Open Test Graphic file"), NULL, 0, CString(strFilter));
	CString filename;
	if (FileDlg.DoModal() == IDOK)
	{
		filename = FileDlg.GetPathName(); // return full path and filename
	}
	else
		return;
	USES_CONVERSION;
	const char* strFilename = T2A((LPCTSTR)filename);
	cv::Mat matTestingNumbers = cv::imread(strFilename, 1);

	if (matTestingNumbers.empty())
	{
		LogStatus("error: image not read from file\n");
		return;
	}

	cv::Mat matGrayscale;
	cv::Mat matBlurred;
	cv::Mat matThresh;
	cv::Mat matThreshCopy;
	/////////////////////////////////////////////
	DrawOnPanel(matTestingNumbers.size().width, matTestingNumbers.size().height, matTestingNumbers);
	////////////////////////////////////////////////////////////////
	cv::cvtColor(matTestingNumbers, matGrayscale, CV_BGR2GRAY);
	cv::GaussianBlur(matGrayscale, matBlurred, cv::Size(5, 5), 0);
	cv::adaptiveThreshold(matBlurred, matThresh, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, 2);
	matThreshCopy = matThresh.clone();

	std::vector<std::vector<cv::Point> > ptContours;
	std::vector<cv::Vec4i> v4iHierarchy;


	////////////////////////////////////////
	std::vector<ContourWithData> allContoursWithData;
	std::vector<ContourWithData> validContoursWithData;

	cv::Mat matClassificationInts;
	cv::FileStorage fsClassifications("classifications.xml", cv::FileStorage::READ);
	if (fsClassifications.isOpened() == false)
	{
		LogStatus("error, unable to open training classifications file, exiting program\n");
		return;
	}
	fsClassifications["classifications"] >> matClassificationInts;
	fsClassifications.release();
	cv::Mat matTrainingImagesAsFlattenedFloats;

	cv::FileStorage fsTrainingImages("images.xml", cv::FileStorage::READ);

	if (fsTrainingImages.isOpened() == false)
	{
		LogStatus("error, unable to open training images file, exiting program\n");
		return;
	}
	fsTrainingImages["images"] >> matTrainingImagesAsFlattenedFloats;
	fsTrainingImages.release();
	cv::Ptr<cv::ml::KNearest>  kNearest(cv::ml::KNearest::create());
	kNearest->train(matTrainingImagesAsFlattenedFloats, cv::ml::ROW_SAMPLE, matClassificationInts);

	//test ///////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////


	cv::findContours(matThreshCopy, ptContours, v4iHierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < ptContours.size(); i++)
	{
		ContourWithData contourWithData;
		contourWithData.ptContour = ptContours[i];
		contourWithData.boundingRect = cv::boundingRect(contourWithData.ptContour);
		contourWithData.fltArea = cv::contourArea(contourWithData.ptContour);
		allContoursWithData.push_back(contourWithData);
	}

	for (int i = 0; i < allContoursWithData.size(); i++)
	{
		if (allContoursWithData[i].checkIfContourIsValid())
		{
			validContoursWithData.push_back(allContoursWithData[i]);
		}
	}
	std::sort(validContoursWithData.begin(), validContoursWithData.end(), ContourWithData::sortByBoundingRectXPosition);

	std::string strFinalString;

	for (int i = 0; i < validContoursWithData.size(); i++)
	{
		cv::rectangle(matTestingNumbers, validContoursWithData[i].boundingRect, cv::Scalar(255, 0, 0), 2);
		cv::Mat matROI = matThresh(validContoursWithData[i].boundingRect);
		cv::Mat matROIResized;
		cv::resize(matROI, matROIResized, cv::Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));
		cv::Mat matROIFloat;
		matROIResized.convertTo(matROIFloat, CV_32FC1);
		cv::Mat matROIFlattenedFloat = matROIFloat.reshape(1, 1);
		cv::Mat matCurrentChar(0, 0, CV_32F);
		kNearest->findNearest(matROIFlattenedFloat, 1, matCurrentChar);
		float fltCurrentChar = (float)matCurrentChar.at<float>(0, 0);
		strFinalString = strFinalString + char(int(fltCurrentChar));
	}

	LogStatus("numbers read = %s\n", strFinalString.c_str());
	matGrayscale.release();
	matBlurred.release();
	matThresh.release();
	matThreshCopy.release();
	cv::waitKey(10);
	cv::destroyAllWindows();
	return;
}

void CSudokuGrabberAndSolverDlg::OnBnClickedReadsudoku()
{
	char strFilter[] = { "All Files (*.*)|*.*|" };
	CFileDialog FileDlg(TRUE, CString("Open Sudoku Graphic file"), NULL, 0, CString(strFilter));
	CString filename;
	if (FileDlg.DoModal() == IDOK)
	{
		filename = FileDlg.GetPathName(); // return full path and filename
	}
	else
		return;
	LogStatus("\n New Sudoku \n");
	USES_CONVERSION;
	const char* strFilename = T2A((LPCTSTR)filename);
	cv::Mat sudoku = cv::imread(strFilename, 1);
	DrawOnPanel(360,360, sudoku);
	//////////////////////////////////////////////////////////////////

	cv::Mat outerBox = cv::Mat(sudoku.size(), CV_8UC1);
	//Preprocessing the image
	cv::cvtColor(sudoku, sudoku, CV_BGR2GRAY);
	GaussianBlur(sudoku, sudoku, cv::Size(5, 5), 0);
	adaptiveThreshold(sudoku, outerBox, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 5, 2);
	bitwise_not(outerBox, outerBox);
	cv::Mat kernel = (cv::Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);
	dilate(outerBox, outerBox, kernel);
	//Finding the biggest blob
	int count = 0;
	int max = -1;

	cv::Point maxPt;

	for (int y = 0; y < outerBox.size().height; y++)
	{
		uchar *row = outerBox.ptr(y);
		for (int x = 0; x < outerBox.size().width; x++)
		{
			if (row[x] >= 128)
			{

				int area = floodFill(outerBox, cv::Point(x, y), CV_RGB(0, 0, 64));

				if (area > max)
				{
					maxPt = cv::Point(x, y);
					max = area;
				}
			}
		}
	}

	floodFill(outerBox, maxPt, CV_RGB(255, 255, 255));

	for (int y = 0; y < outerBox.size().height; y++)
	{
		uchar *row = outerBox.ptr(y);
		for (int x = 0; x < outerBox.size().width; x++)
		{
			if (row[x] == 64 && x != maxPt.x && y != maxPt.y)
			{
				int area = floodFill(outerBox, cv::Point(x, y), CV_RGB(0, 0, 0));
			}
		}
	}
	erode(outerBox, outerBox, kernel);

	//Detecting lines
	std::vector<cv::Vec2f> lines;
	HoughLines(outerBox, lines, 1, CV_PI / 180, 200);
	mergeRelatedLines(&lines, sudoku); // Add this line
	for (int i = 0; i < lines.size(); i++)
	{
		drawLine(lines[i], outerBox, CV_RGB(0, 0, 128));
	}
	//Finding extreme lines
	// Now detect the lines on extremes
	cv::Vec2f topEdge = cv::Vec2f(1000, 1000);
	double topYIntercept = 100000, topXIntercept = 0;
	cv::Vec2f bottomEdge = cv::Vec2f(-1000, -1000);
	double bottomYIntercept = 0, bottomXIntercept = 0;
	cv::Vec2f leftEdge = cv::Vec2f(1000, 1000);
	double leftXIntercept = 100000, leftYIntercept = 0;
	cv::Vec2f rightEdge = cv::Vec2f(-1000, -1000);
	double rightXIntercept = 0, rightYIntercept = 0;

	for (int i = 0; i < lines.size(); i++)
	{

		cv::Vec2f current = lines[i];

		float p = current[0];
		float theta = current[1];

		if (p == 0 && theta == -100)
			continue;

		double xIntercept, yIntercept;
		xIntercept = p / cos(theta);
		yIntercept = p / (cos(theta)*sin(theta));

		if (theta > CV_PI * 80 / 180 && theta < CV_PI * 100 / 180)
		{
			if (p < topEdge[0])
				topEdge = current;

			if (p > bottomEdge[0])
				bottomEdge = current;
		}
		else if (theta<CV_PI * 10 / 180 || theta>CV_PI * 170 / 180)
		{
			if (xIntercept > rightXIntercept)
			{
				rightEdge = current;
				rightXIntercept = xIntercept;
			}
			else if (xIntercept <= leftXIntercept)
			{
				leftEdge = current;
				leftXIntercept = xIntercept;
			}
		}
	}
	drawLine(topEdge, sudoku, CV_RGB(0, 0, 0));
	drawLine(bottomEdge, sudoku, CV_RGB(0, 0, 0));
	drawLine(leftEdge, sudoku, CV_RGB(0, 0, 0));
	drawLine(rightEdge, sudoku, CV_RGB(0, 0, 0));

	cv::Point left1, left2, right1, right2, bottom1, bottom2, top1, top2;

	int height = outerBox.size().height;
	int width = outerBox.size().width;

	if (leftEdge[1] != 0)
	{
		left1.x = 0;        left1.y = leftEdge[0] / sin(leftEdge[1]);
		left2.x = width;    left2.y = -left2.x / tan(leftEdge[1]) + left1.y;
	}
	else
	{
		left1.y = 0;        left1.x = leftEdge[0] / cos(leftEdge[1]);
		left2.y = height;    left2.x = left1.x - height * tan(leftEdge[1]);

	}

	if (rightEdge[1] != 0)
	{
		right1.x = 0;        right1.y = rightEdge[0] / sin(rightEdge[1]);
		right2.x = width;    right2.y = -right2.x / tan(rightEdge[1]) + right1.y;
	}
	else
	{
		right1.y = 0;        right1.x = rightEdge[0] / cos(rightEdge[1]);
		right2.y = height;    right2.x = right1.x - height * tan(rightEdge[1]);

	}

	bottom1.x = 0;    bottom1.y = bottomEdge[0] / sin(bottomEdge[1]);
	bottom2.x = width; bottom2.y = -bottom2.x / tan(bottomEdge[1]) + bottom1.y;

	top1.x = 0;        top1.y = topEdge[0] / sin(topEdge[1]);
	top2.x = width;    top2.y = -top2.x / tan(topEdge[1]) + top1.y;

	// Next, we find the intersection of  these four lines
	double leftA = left2.y - left1.y;
	double leftB = left1.x - left2.x;

	double leftC = leftA * left1.x + leftB * left1.y;

	double rightA = right2.y - right1.y;
	double rightB = right1.x - right2.x;

	double rightC = rightA * right1.x + rightB * right1.y;

	double topA = top2.y - top1.y;
	double topB = top1.x - top2.x;

	double topC = topA * top1.x + topB * top1.y;

	double bottomA = bottom2.y - bottom1.y;
	double bottomB = bottom1.x - bottom2.x;

	double bottomC = bottomA * bottom1.x + bottomB * bottom1.y;

	// Intersection of left and top
	double detTopLeft = leftA * topB - leftB * topA;

	CvPoint ptTopLeft = cvPoint((topB*leftC - leftB * topC) / detTopLeft, (leftA*topC - topA * leftC) / detTopLeft);

	// Intersection of top and right
	double detTopRight = rightA * topB - rightB * topA;

	CvPoint ptTopRight = cvPoint((topB*rightC - rightB * topC) / detTopRight, (rightA*topC - topA * rightC) / detTopRight);

	// Intersection of right and bottom
	double detBottomRight = rightA * bottomB - rightB * bottomA;
	CvPoint ptBottomRight = cvPoint((bottomB*rightC - rightB * bottomC) / detBottomRight, (rightA*bottomC - bottomA * rightC) / detBottomRight);// Intersection of bottom and left
	double detBottomLeft = leftA * bottomB - leftB * bottomA;
	CvPoint ptBottomLeft = cvPoint((bottomB*leftC - leftB * bottomC) / detBottomLeft, (leftA*bottomC - bottomA * leftC) / detBottomLeft);

	int maxLength = (ptBottomLeft.x - ptBottomRight.x)*(ptBottomLeft.x - ptBottomRight.x) + (ptBottomLeft.y - ptBottomRight.y)*(ptBottomLeft.y - ptBottomRight.y);
	int temp = (ptTopRight.x - ptBottomRight.x)*(ptTopRight.x - ptBottomRight.x) + (ptTopRight.y - ptBottomRight.y)*(ptTopRight.y - ptBottomRight.y);

	if (temp > maxLength)
		maxLength = temp;

	temp = (ptTopRight.x - ptTopLeft.x)*(ptTopRight.x - ptTopLeft.x) + (ptTopRight.y - ptTopLeft.y)*(ptTopRight.y - ptTopLeft.y);

	if (temp > maxLength)
		maxLength = temp;

	temp = (ptBottomLeft.x - ptTopLeft.x)*(ptBottomLeft.x - ptTopLeft.x) + (ptBottomLeft.y - ptTopLeft.y)*(ptBottomLeft.y - ptTopLeft.y);

	if (temp > maxLength)
		maxLength = temp;

	maxLength = sqrt((double)maxLength);

	cv::Point2f src[4], dst[4];
	src[0] = ptTopLeft;
	dst[0] = cv::Point2f(0, 0);
	src[1] = ptTopRight;
	dst[1] = cv::Point2f(maxLength - 1, 0);
	src[2] = ptBottomRight;
	dst[2] = cv::Point2f(maxLength - 1, maxLength - 1);
	src[3] = ptBottomLeft;
	dst[3] = cv::Point2f(0, maxLength - 1);

	cv::Mat undistorted = cv::Mat(cv::Size(maxLength, maxLength), CV_8UC1);
	cv::warpPerspective(sudoku, undistorted, cv::getPerspectiveTransform(src, dst), cv::Size(maxLength, maxLength));
	///////////////////////
	cv::Mat undistortedThreshed = undistorted.clone();
	adaptiveThreshold(undistorted, undistortedThreshed, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 11, 2);
//	cv::imshow("temp0", undistortedThreshed);
//	cvWaitKey(1000);
	//////////////////////////
	// remove guidline
	{
		cv::Mat outerBox1 = undistortedThreshed.clone();
		dilate(outerBox1, outerBox1, kernel);
		int count = 0;
		int max = -1;
		cv::Point maxPt;
		for (int y = 0; y < outerBox1.size().height; y++)
		{
			uchar *row = outerBox1.ptr(y);
			for (int x = 0; x < outerBox1.size().width; x++)
			{
				if (row[x] >= 128)
				{
					int area = floodFill(outerBox1, cv::Point(x, y), CV_RGB(0, 0, 64));
					if (area > max)
					{
						maxPt = cv::Point(x, y);
						max = area;
					}
				}
			}
		}

		floodFill(outerBox1, maxPt, CV_RGB(255, 255, 255));
		for (int y = 0; y < outerBox1.size().height; y++)
		{
			uchar *row = outerBox1.ptr(y);
			for (int x = 0; x < outerBox1.size().width; x++)
			{
				if (row[x] == 64 && x != maxPt.x && y != maxPt.y)
				{
					int area = floodFill(outerBox1, cv::Point(x, y), CV_RGB(0, 0, 0));
				}
			}
		}
		erode(outerBox1, outerBox1, kernel);
		bitwise_not(outerBox1, outerBox1);
		bitwise_and(undistortedThreshed, outerBox1, undistortedThreshed);
	}
	//////////////////////////
	std::vector<ContourWithData> allContoursWithData;
	std::vector<ContourWithData> validContoursWithData;
	cv::Mat matClassificationInts;
	cv::FileStorage fsClassifications("E:\\ProjectForFun\\datafiles\\first\\classifications.xml", cv::FileStorage::READ);
	if (fsClassifications.isOpened() == false)
	{
		LogStatus("error, unable to open training classifications file, exiting program\n");
		return;
	}
	fsClassifications["classifications"] >> matClassificationInts;
	fsClassifications.release();
	cv::Mat matTrainingImagesAsFlattenedFloats;
	cv::FileStorage fsTrainingImages("E:\\ProjectForFun\\datafiles\\first\\images.xml", cv::FileStorage::READ);
	if (fsTrainingImages.isOpened() == false)
	{
		LogStatus("error, unable to open training images file, exiting program\n");
		return;
	}
	fsTrainingImages["images"] >> matTrainingImagesAsFlattenedFloats;
	fsTrainingImages.release();
	cv::Ptr<cv::ml::KNearest>  kNearest(cv::ml::KNearest::create());
	kNearest->train(matTrainingImagesAsFlattenedFloats, cv::ml::ROW_SAMPLE, matClassificationInts);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// make it saperate.
	int dist = ceil((double)maxLength / 9);
	cv::Mat currentCell = cv::Mat(dist, dist, CV_8UC1);
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9;j++)
			m_nValues[i][j] = 0;
	for (int j = 0; j<9; j++)
	{
		for (int i = 0; i < 9; i++)
		{
			validContoursWithData.clear();
			allContoursWithData.clear();

			for (int y = 0; y < dist && j*dist + y < undistortedThreshed.cols; y++)
			{

				uchar* ptr = currentCell.ptr(y);

				for (int x = 0; x < dist && i*dist + x < undistortedThreshed.rows; x++)
				{
					ptr[x] = undistortedThreshed.at<uchar>(j*dist + y, i*dist + x);
				}
			}
			cv::Mat newCurrentCell = currentCell.clone();

			std::vector<std::vector<cv::Point> > ptContours;
			std::vector<cv::Vec4i> v4iHierarchy;

			cv::findContours(newCurrentCell, ptContours, v4iHierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

			if (ptContours.size() == 1)
			{
				ContourWithData contourWithData;
				contourWithData.ptContour = ptContours[0];
				contourWithData.boundingRect = cv::boundingRect(contourWithData.ptContour);
				contourWithData.fltArea = cv::contourArea(contourWithData.ptContour);
				allContoursWithData.push_back(contourWithData);
			}
			else if (ptContours.size() > 1)
			{// max area 
				float fltMax = 0.0;
				int nMaxi = 0;
				for (int i = 0; i < ptContours.size(); i++)	// can only one ptContour
				{
					ContourWithData contourWithData;
					contourWithData.ptContour = ptContours[i];
					contourWithData.boundingRect = cv::boundingRect(contourWithData.ptContour);
					contourWithData.fltArea = cv::contourArea(contourWithData.ptContour);
					if (contourWithData.fltArea > fltMax)
					{
						fltMax = contourWithData.fltArea;
						nMaxi = i;
					}
				}
				ContourWithData contourWithData;
				contourWithData.ptContour = ptContours[nMaxi];
				contourWithData.boundingRect = cv::boundingRect(contourWithData.ptContour);
				contourWithData.fltArea = cv::contourArea(contourWithData.ptContour);
				allContoursWithData.push_back(contourWithData);
			}
			else
			{
				LogStatus("0");
				continue;
			}

			for (int i = 0; i < allContoursWithData.size(); i++)
			{
				if (allContoursWithData[i].checkIfContourIsValid())
				{
					validContoursWithData.push_back(allContoursWithData[i]);
				}
				else
					LogStatus("0");
			}
			if (validContoursWithData.size() != 0)
			{
				cv::Mat matROI = newCurrentCell(validContoursWithData[0].boundingRect);
				cv::Mat matROIResized;
				cv::resize(matROI, matROIResized, cv::Size(RESIZED_IMAGE_WIDTH, RESIZED_IMAGE_HEIGHT));
				cv::Mat matROIFloat;
				matROIResized.convertTo(matROIFloat, CV_32FC1);
				cv::Mat matROIFlattenedFloat = matROIFloat.reshape(1, 1);
				cv::Mat matCurrentChar(0, 0, CV_32F);
				kNearest->findNearest(matROIFlattenedFloat, 1, matCurrentChar);
				float fltCurrentChar = (float)matCurrentChar.at<float>(0, 0);
				int nCurchar = int(fltCurrentChar) - 48;
				m_nValues[j][i] = nCurchar;
				LogStatus("%d", nCurchar);
			}
		}
		LogStatus("\n");
	}
	UpdateData(false);
}
void CSudokuGrabberAndSolverDlg::OnBnClickedSolvesudoku()
{
	ClearPossibleAnswer();
	DWORD nStartTIme = GetTickCount();
	int nFixedCount = SetNumbers();
	LogStatus("Done in 1st pahse. %d ms \n", GetTickCount() - nStartTIme);
	if (nFixedCount < 81)
	{
		//snaphot
		memcpy(snapShotValues, m_nValues, 4 * 81);
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				snapshotPossibleAnswer[i][j] = possibleAnswer[i][j];

		//guess//
		bool bSetAll = false;
		while (1)
		{
			int nGuessi, nGuessj;
			bool bFound = false;
			int nStartSmallCount = 2;
			while (nStartSmallCount < 4)
			{
				for (int i = 0; i < 9; i++) {
					for (int j = 0; j < 9; j++) {
						if (possibleAnswer[i][j].size() == nStartSmallCount)
						{
							nGuessi = i;
							nGuessj = j;
							bFound = true;
							break;
						}
					}
					if (bFound)
						break;
				}
				if (bFound)
					break;
				nStartSmallCount++;
			}
			if (bFound == false)
			{
				LogStatus("Can't solve it. %d ms \n", GetTickCount() - nStartTIme);
				break;
			}
			std::vector<int> tempVec = possibleAnswer[nGuessi][nGuessj];
			for (std::vector<int>::iterator it = tempVec.begin(); it != tempVec.end(); ++it)
			{
				m_nValues[nGuessi][nGuessj] = *it;
				nFixedCount = SetNumbers();
				if (nFixedCount == 81)
				{
					bSetAll = true;
					break;
				}
				else
				{
					memcpy(m_nValues, snapShotValues, 4 * 81);
					for (int i = 0; i < 9; i++)
						for (int j = 0; j < 9; j++)
							possibleAnswer[i][j] = snapshotPossibleAnswer[i][j];
				}
			}
			if (bSetAll)
			{
				LogStatus("Done in 2nd pahse. %d ms \n", GetTickCount() - nStartTIme);
				break;
			}
		}
	}
}
void CSudokuGrabberAndSolverDlg::drawLine(cv::Vec2f line, cv::Mat &img, cv::Scalar rgb)
{
	if (line[1] != 0)
	{
		float m = -1 / tan(line[1]);

		float c = line[0] / sin(line[1]);

		cv::line(img, cv::Point(0, c), cv::Point(img.size().width, m*img.size().width + c), rgb);
	}
	else
	{
		cv::line(img, cv::Point(line[0], 0), cv::Point(line[0], img.size().height), rgb);
	}

}
void CSudokuGrabberAndSolverDlg::mergeRelatedLines(std::vector<cv::Vec2f> *lines, cv::Mat &img)
{
	std::vector<cv::Vec2f>::iterator current;
	for (current = lines->begin(); current != lines->end(); current++)
	{
		if ((*current)[0] == 0 && (*current)[1] == -100)
			continue;
		float p1 = (*current)[0];
		float theta1 = (*current)[1];
		cv::Point pt1current, pt2current;
		if (theta1 > CV_PI * 45 / 180 && theta1 < CV_PI * 135 / 180)
		{
			pt1current.x = 0;

			pt1current.y = p1 / sin(theta1);

			pt2current.x = img.size().width;
			pt2current.y = -pt2current.x / tan(theta1) + p1 / sin(theta1);
		}
		else
		{
			pt1current.y = 0;

			pt1current.x = p1 / cos(theta1);

			pt2current.y = img.size().height;
			pt2current.x = -pt2current.y / tan(theta1) + p1 / cos(theta1);

		}
		std::vector<cv::Vec2f>::iterator    pos;
		for (pos = lines->begin(); pos != lines->end(); pos++)
		{
			if (*current == *pos) continue;
			if (fabs((*pos)[0] - (*current)[0]) < 20 && fabs((*pos)[1] - (*current)[1]) < CV_PI * 10 / 180)
			{
				float p = (*pos)[0];
				float theta = (*pos)[1];
				cv::Point pt1, pt2;
				if ((*pos)[1] > CV_PI * 45 / 180 && (*pos)[1] < CV_PI * 135 / 180)
				{
					pt1.x = 0;
					pt1.y = p / sin(theta);
					pt2.x = img.size().width;
					pt2.y = -pt2.x / tan(theta) + p / sin(theta);
				}
				else
				{
					pt1.y = 0;
					pt1.x = p / cos(theta);
					pt2.y = img.size().height;
					pt2.x = -pt2.y / tan(theta) + p / cos(theta);
				}
				if (((double)(pt1.x - pt1current.x)*(pt1.x - pt1current.x) + (pt1.y - pt1current.y)*(pt1.y - pt1current.y) < 64 * 64) &&
					((double)(pt2.x - pt2current.x)*(pt2.x - pt2current.x) + (pt2.y - pt2current.y)*(pt2.y - pt2current.y) < 64 * 64))
				{
					// Merge the two
					(*current)[0] = ((*current)[0] + (*pos)[0]) / 2;

					(*current)[1] = ((*current)[1] + (*pos)[1]) / 2;

					(*pos)[0] = 0;
					(*pos)[1] = -100;
				}
			}
		}
	}
}
void CSudokuGrabberAndSolverDlg::OnBnClickedExit()
{
	OnOK();
}
int CSudokuGrabberAndSolverDlg::SetNumbers()
{
	int nFixedCount = 0;
	int lastCount = 0;
	while (nFixedCount < 81)
	{
		nFixedCount = CheckPossibleAnswer();
		CheckHorizontalUnique();
		CheckPossibleAnswer();
		CheckVerticalUnique();
		CheckPossibleAnswer();
		CheckSqaureUnique();
		nFixedCount = CheckPossibleAnswer();
		PrintCurrent();
		if (lastCount != nFixedCount)
			lastCount = nFixedCount;
		else
			break;
	}
	return nFixedCount;
}
int CSudokuGrabberAndSolverDlg::CheckPossibleAnswer()
{
	int nIter = 0;
	int nFixedCount = 0;
	while (1)
	{
		LogStatus("%d time iteration\n", ++nIter);
		int nNewCount = 0;
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{

				if (m_nValues[i][j] != 0)
				{
					possibleAnswer[i][j].clear();
					nNewCount++;
				}
				else
				{
					//check horizontal
					for (int k = 0; k < 9; k++)
					{
						if (m_nValues[i][k] != 0)
						{
							std::vector<int>::iterator it = std::find(possibleAnswer[i][j].begin(), possibleAnswer[i][j].end(), m_nValues[i][k]);
							if (it != possibleAnswer[i][j].end())
								possibleAnswer[i][j].erase(it);

						}
					}
					//check vertical, 
					for (int k = 0; k < 9; k++)
					{
						if (m_nValues[k][j] != 0)
						{
							std::vector<int>::iterator it = std::find(possibleAnswer[i][j].begin(), possibleAnswer[i][j].end(), m_nValues[k][j]);
							if (it != possibleAnswer[i][j].end())
								possibleAnswer[i][j].erase(it);
						}
					}
					//check square
					for (int k = (i / 3 * 3); k < (i / 3 * 3 + 3); k++)
					{
						for (int l = (j / 3 * 3); l < (j / 3 * 3 + 3); l++)
						{
							if (m_nValues[k][l] != 0)
							{
								std::vector<int>::iterator it = std::find(possibleAnswer[i][j].begin(), possibleAnswer[i][j].end(), m_nValues[k][l]);
								if (it != possibleAnswer[i][j].end())
									possibleAnswer[i][j].erase(it);
							}
						}
					}
				}
			}
		}
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (possibleAnswer[i][j].size() == 1)
				{
					m_nValues[i][j] = possibleAnswer[i][j].at(0);
				}
			}
		}
		UpdateData(false);
		if (nNewCount == nFixedCount)
			break;
		else
			nFixedCount = nNewCount;
	}
	return nFixedCount;
}
void CSudokuGrabberAndSolverDlg::CheckHorizontalUnique()
{
	for (int i = 0; i < 9; i++) //check horizontal
	{
		std::map<int, int> notSettles;
		for (int k = 1; k <= 9; k++)
			notSettles[k] = 0;
		for (int j = 0; j < 9; j++)
		{
			for (std::vector<int>::iterator it = possibleAnswer[i][j].begin(); it != possibleAnswer[i][j].end(); ++it)
				notSettles[*it]++;
		}
		std::vector<int> canFixNumbers;
		for (std::map<int, int>::iterator it = notSettles.begin(); it != notSettles.end(); ++it)
		{
			if (it->second == 1)
			{
				canFixNumbers.push_back(it->first);
			}
		}
		if (canFixNumbers.size() > 0)
		{
			bool bOutToNextLoop = false;
			for (std::vector<int>::iterator itFixNumber = canFixNumbers.begin(); itFixNumber != canFixNumbers.end(); ++itFixNumber)
			{
				for (int j = 0; j < 9; j++)
				{
					for (std::vector<int>::iterator it = possibleAnswer[i][j].begin(); it != possibleAnswer[i][j].end(); ++it)
					{
						if (*it == *itFixNumber)
						{
							m_nValues[i][j] = *it;
							possibleAnswer[i][j].clear();
							bOutToNextLoop = true;
							LogStatus("%d %d is %d in horizontal check.\n", i + 1, j + 1, m_nValues[i][j]);
							break;
						}
					}
					if (bOutToNextLoop)
						break;
				}
			}
		}
	}
}
void CSudokuGrabberAndSolverDlg::CheckVerticalUnique()
{
	for (int j = 0; j < 9; j++)//check vertical
	{
		std::map<int, int> notSettles;
		for (int k = 1; k <= 9; k++)
			notSettles[k] = 0;
		for (int i = 0; i < 9; i++)
		{
			for (std::vector<int>::iterator it = possibleAnswer[i][j].begin(); it != possibleAnswer[i][j].end(); ++it)
				notSettles[*it]++;
		}
		std::vector<int> canFixNumbers;
		for (std::map<int, int>::iterator it = notSettles.begin(); it != notSettles.end(); ++it)
		{
			if (it->second == 1)
			{
				canFixNumbers.push_back(it->first);
			}
		}
		if (canFixNumbers.size() > 0)
		{
			bool bOutToNextLoop = false;
			for (std::vector<int>::iterator itFixNumber = canFixNumbers.begin(); itFixNumber != canFixNumbers.end(); ++itFixNumber)
			{
				for (int i = 0; i < 9; i++)
				{
					for (std::vector<int>::iterator it = possibleAnswer[i][j].begin(); it != possibleAnswer[i][j].end(); ++it)
					{
						if (*it == *itFixNumber)
						{
							m_nValues[i][j] = *it;
							possibleAnswer[i][j].clear();
							bOutToNextLoop = true;
							LogStatus("%d %d is %d in vertical check.\n", i + 1, j + 1, m_nValues[i][j]);
							break;
						}
					}
					if (bOutToNextLoop)
						break;
				}
			}
		}
	}
}
void CSudokuGrabberAndSolverDlg::CheckSqaureUnique()
{													/*0 3 6*/
	for (int section = 0; section < 9; section++)	/*1 4 7*/
	{												/*2 5 8*/
		std::map<int, int> notSettles;
		for (int k = 1; k <= 9; k++)
			notSettles[k] = 0;
		for (int i = (section % 3) * 3; i < ((section % 3) * 3 + 3); i++) //check Square
		{
			for (int j = ((section / 3) * 3); j < ((section / 3) * 3 + 3); j++)
			{
				for (std::vector<int>::iterator it = possibleAnswer[i][j].begin(); it != possibleAnswer[i][j].end(); ++it)
					notSettles[*it]++;
			}
		}
		std::vector<int> canFixNumbers;
		for (std::map<int, int>::iterator it = notSettles.begin(); it != notSettles.end(); ++it)
		{
			if (it->second == 1)
			{
				canFixNumbers.push_back(it->first);
			}
		} //till here.....
		if (canFixNumbers.size() > 0)
		{
			bool bOutToNextLoop = false;
			for (std::vector<int>::iterator itFixNumber = canFixNumbers.begin(); itFixNumber != canFixNumbers.end(); ++itFixNumber)
			{
				for (int i = (section % 3) * 3; i < ((section % 3) * 3 + 3); i++) //check Square
				{
					for (int j = ((section / 3) * 3); j < ((section / 3) * 3 + 3); j++)
					{
						for (std::vector<int>::iterator it = possibleAnswer[i][j].begin(); it != possibleAnswer[i][j].end(); ++it)
						{
							if (*it == *itFixNumber)
							{
								m_nValues[i][j] = *it;
								possibleAnswer[i][j].clear();
								bOutToNextLoop = true;
								LogStatus("%d %d is %d in square check.\n", i + 1, j + 1, m_nValues[i][j]);
								break;
							}
						}
						if (bOutToNextLoop)
							break;
					}
				}
			}
		}
	}
}
void CSudokuGrabberAndSolverDlg::ClearPossibleAnswer()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			possibleAnswer[i][j] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		}
	}
}
void CSudokuGrabberAndSolverDlg::PrintCurrent()
{
	LogStatus("\n");
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			LogStatus("%d ", m_nValues[i][j]);
		}
		LogStatus("\n");
	}
}