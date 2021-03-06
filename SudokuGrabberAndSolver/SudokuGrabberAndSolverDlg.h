
// SudokuGrabberAndSolverDlg.h : header file
//
// referenced https://github.com/MicrocontrollersAndMore/OpenCV_3_KNN_Character_Recognition_Cpp for digit recognition.
#pragma once
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/ml/ml.hpp>
#define CV_RGB( r, g, b )  cvScalar( (b), (g), (r), 0 )

using namespace cv;

// CSudokuGrabberAndSolverDlg dialog
class CSudokuGrabberAndSolverDlg : public CDialog
{
	enum {
		MIN_CONTOUR_AREA = 30
	};
	enum {
		RESIZED_IMAGE_WIDTH = 20, RESIZED_IMAGE_HEIGHT = 30
	};

// Construction
public:
	CSudokuGrabberAndSolverDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUDOKUGRABBERANDSOLVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void	DrawOnPanel(int OriginXsize, int OriginYSize, cv::Mat dataGraphic);
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void LogStatus(char*  text, ...);

	void mergeRelatedLines(std::vector<cv::Vec2f> *lines, cv::Mat &img);
		void drawLine(cv::Vec2f line, cv::Mat &img, cv::Scalar rgb);
	CImage* m_pImg;
	CEdit m_LogBox;
	CStatic m_Panel;
public:
	afx_msg void OnBnClickedTraining();
	afx_msg void OnBnClickedTesttraining();
	afx_msg void OnBnClickedReadsudoku();
	afx_msg void OnBnClickedSolvesudoku();
	afx_msg void OnBnClickedExit();

	int	 CheckPossibleAnswer();
	void CheckHorizontalUnique();
	void CheckVerticalUnique();
	void CheckSqaureUnique();
	void ClearPossibleAnswer();
	void PrintCurrent();
	int  SetNumbers();


	int m_nValues[9][9];
	int snapShotValues[9][9];
	std::vector<int> possibleAnswer[9][9];
	std::vector<int> snapshotPossibleAnswer[9][9];

};

class ContourWithData {
public:
	enum {
		MIN_CONTOUR_AREA = 30
	};
	// member variables ///////////////////////////////////////////////////////////////////////////
	std::vector<cv::Point> ptContour;           // contour
	cv::Rect boundingRect;                      // bounding rect for contour
	float fltArea;                              // area of contour
												///////////////////////////////////////////////////////////////////////////////////////////////
	bool checkIfContourIsValid() {                              // obviously in a production grade program
		if (fltArea < MIN_CONTOUR_AREA) return false;           // we would have a much more robust function for 
		return true;                                            // identifying if a contour is valid !!
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	static bool sortByBoundingRectXPosition(const ContourWithData& cwdLeft, const ContourWithData& cwdRight) {      // this function allows us to sort
		return(cwdLeft.boundingRect.x < cwdRight.boundingRect.x);                                                   // the contours from left to right
	}
};