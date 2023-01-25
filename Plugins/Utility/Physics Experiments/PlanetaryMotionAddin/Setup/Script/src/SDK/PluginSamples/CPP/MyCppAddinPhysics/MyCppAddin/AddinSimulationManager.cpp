// AddinSimulationManager.cpp : implementation file
//

#include "stdafx.h"
#include "MySimulationAddin.h"
#include "AddinSimulationManager.h"


// CAddinSimulationManager

CAddinSimulationManager::CAddinSimulationManager(CMySimulationAddin* pAddin)
{
	m_pAddin = pAddin;
	m_pPlusTwoPhysicsExperiment = new CPlusTwoPhysicsExperiment(this);
	m_pBScPhysicsExperiment = new CBScPhysicsExperiment(this);
	m_pMScPhysicsExperiment = new CMScPhysicsExperiment(this);
	m_strExperimentType = CPP_SAMPLE_EXPERIMENT_TYPE_PLUS_TWO;
	m_bSimulationActive = FALSE;
	
}

CAddinSimulationManager::~CAddinSimulationManager()
{
	delete m_pPlusTwoPhysicsExperiment;
	delete m_pBScPhysicsExperiment;
	delete m_pMScPhysicsExperiment;

}

void CAddinSimulationManager::LoadExperiments(CString ExperimentType)
{
	if (!IsAddinExperimentSelected())
	{
		return;
	}

	if (ExperimentType == CPP_SAMPLE_EXPERIMENT_TYPE_PLUS_TWO)
	{
		m_pPlusTwoPhysicsExperiment->LoadAllExperiments();
	}
	else if (ExperimentType == CPP_SAMPLE_EXPERIMENT_TYPE_BSC)
	{
		m_pBScPhysicsExperiment->LoadAllExperiments();
	}
	else if (ExperimentType == CPP_SAMPLE_EXPERIMENT_TYPE_MSC)
	{
		m_pMScPhysicsExperiment->LoadAllExperiments();
	}
	m_strExperimentType = ExperimentType;
}

void CAddinSimulationManager::OnTreeNodeSelect(long SessionID, BSTR RootText, BSTR ExperimentGroup, BSTR ExperimentName)
{
	if (m_pAddin->m_lSessionID != SessionID)
	{
		return;
	}
	else
	{
		m_strRootText = RootText;
		m_strExperimentGroup = ExperimentGroup;
		m_strExperimentName = ExperimentName;
		SetStatusBarMessage(CString(RootText)+ CString(" | ") + CString(ExperimentGroup) + CString(" | ") + CString(ExperimentName), FALSE);
	}
	if (CString(RootText) == CPP_SAMPLE_EXPERIMENT_TYPE_BSC_PROPERTIES)
	{
		m_pBScPhysicsExperiment->OnTreeNodeSelect(ExperimentGroup, ExperimentName);
	}
	else if (CString(RootText) == CPP_SAMPLE_EXPERIMENT_TYPE_MSC_PROPERTIES)
	{
		m_pMScPhysicsExperiment->OnTreeNodeSelect(ExperimentGroup, ExperimentName);
	}
	else if (CString(RootText) == CPP_SAMPLE_EXPERIMENT_TYPE_PLUS_TWO_PROPERTIES)
	{
		m_pPlusTwoPhysicsExperiment->OnTreeNodeSelect(ExperimentGroup, ExperimentName);
	}
}
void CAddinSimulationManager::OnTreeNodeDblClick(long SessionID, BSTR RootText, BSTR ExperimentGroup, BSTR ExperimentName)
{
	if (m_pAddin->m_lSessionID != SessionID)
	{
		return;
	}
	else
	{
		m_strRootText = RootText;
		m_strExperimentGroup = ExperimentGroup;
		m_strExperimentName = ExperimentName;
		SetStatusBarMessage(CString(RootText) + CString(" | ") + CString(ExperimentGroup) + CString(" | ") + CString(ExperimentName), FALSE);
	}

	if (CString(RootText) == CPP_SAMPLE_EXPERIMENT_TYPE_BSC_PROPERTIES)
	{
		m_pBScPhysicsExperiment->OnTreeNodeDblClick(ExperimentGroup, ExperimentName);
	}
	else if (CString(RootText) == CPP_SAMPLE_EXPERIMENT_TYPE_MSC_PROPERTIES)
	{
		m_pMScPhysicsExperiment->OnTreeNodeDblClick(ExperimentGroup, ExperimentName);
	}
	else if (CString(RootText) == CPP_SAMPLE_EXPERIMENT_TYPE_PLUS_TWO_PROPERTIES)
	{
		m_pPlusTwoPhysicsExperiment->OnTreeNodeDblClick(ExperimentGroup, ExperimentName);
	}

}
void CAddinSimulationManager::OnReloadExperiment(long SessionID, BSTR RootText, BSTR ExperimentGroup, BSTR ExperimentName)
{
	if (m_pAddin->m_lSessionID != SessionID)
	{
		return;
	}
	if (CString(RootText) == CPP_SAMPLE_EXPERIMENT_TYPE_BSC_PROPERTIES)
	{
		m_pBScPhysicsExperiment->OnReloadExperiment(ExperimentGroup, ExperimentName);
	}
	else if (CString(RootText) == CPP_SAMPLE_EXPERIMENT_TYPE_MSC_PROPERTIES)
	{
		m_pMScPhysicsExperiment->OnReloadExperiment(ExperimentGroup, ExperimentName);
	}
	else if (CString(RootText) == CPP_SAMPLE_EXPERIMENT_TYPE_PLUS_TWO_PROPERTIES)
	{
		m_pPlusTwoPhysicsExperiment->OnReloadExperiment(ExperimentGroup, ExperimentName);
	}
	
}

void CAddinSimulationManager::OnStatusChange(long StatusCode, BSTR StatusDesc, BSTR AdditionalParam)
{
	if (StatusCode == 10000) //No Experiment Selected
	{
		
	}
	else if (StatusCode == 10001 && m_pAddin->m_lSessionID==_ttoi(CString(AdditionalParam))) //Experiment Changed 
	{
		if (m_strExperimentType == CPP_SAMPLE_EXPERIMENT_TYPE_BSC)
		{
			m_pBScPhysicsExperiment->LoadAllExperiments();
			m_pMScPhysicsExperiment->OnReloadExperiment(m_strExperimentGroup.AllocSysString(), m_strExperimentName.AllocSysString());
		}
		else if (m_strExperimentType == CPP_SAMPLE_EXPERIMENT_TYPE_MSC)
		{
			m_pMScPhysicsExperiment->LoadAllExperiments();
			m_pMScPhysicsExperiment->OnReloadExperiment(m_strExperimentGroup.AllocSysString(), m_strExperimentName.AllocSysString());
			
		}
		else if (m_strExperimentType == CPP_SAMPLE_EXPERIMENT_TYPE_PLUS_TWO)
		{
			m_pPlusTwoPhysicsExperiment->LoadAllExperiments();
			m_pPlusTwoPhysicsExperiment->OnReloadExperiment(m_strExperimentGroup.AllocSysString(), m_strExperimentName.AllocSysString());
		}
		m_pAddin->SetRibbonControlText(CString(_T("Cpp.Sample.Experimental.Setup.Select.Experiment")), m_strExperimentType);
		
		m_strSelectedExperiment = StatusDesc;
		LoadDefaultSelection();
	}
}
// CAddinSimulationManager member functions


void CAddinSimulationManager::ResetExpermentTree()
{
	CComPtr<IExperimentTreeView> ExperimentTreeView;
	HRESULT HR = ExperimentTreeView.CoCreateInstance(CLSID_ExperimentTreeView);
	if (FAILED(HR))
	{
		return;
	}
	long SessionID = m_pAddin->m_lSessionID;
	ExperimentTreeView->DeleteAllExperiments(SessionID);
	ExperimentTreeView->SetRootNodeName(CString(_T("Experiment List | Properties")).AllocSysString(), TRUE);
	ExperimentTreeView->Refresh();
}

void CAddinSimulationManager::OnError(long ErrorCode, BSTR ErrorDesc, BSTR AdditionalParam)
{

}
HRESULT CAddinSimulationManager::OnStartSimulation(BSTR ExperimentName)
{
	if (!IsAddinExperimentSelected() || m_strExperimentType == _T(""))
	{
		CString strMessage;
		strMessage.Format(_T("Failed to Start %s. Please select Experimental Parameters before starting the Simulation Experiment.."), CString(ExperimentName));
		AfxMessageBox(strMessage);
		return E_FAIL;
	}
	LoadOtherSimulationOptions();
	ResetAllStatusWindows();
	if (m_strExperimentType == CPP_SAMPLE_EXPERIMENT_TYPE_PLUS_TWO)
	{
		m_pPlusTwoPhysicsExperiment->StartSimulation(m_strExperimentGroup.AllocSysString(), m_strExperimentName.AllocSysString());
	}
	return S_OK;
}

void CAddinSimulationManager::AddOperationStatus(CString strStatus, LONG bPostMessage)
{
	//((CMainFrame*)AfxGetMainWnd())->AddOperationStatus(strStatus, Mode);
	_tcscpy(m_strCurrentOutputStatusMessage, strStatus);

	//Send Operation Status to Window
	CComPtr<IMainWindow> MainWindow;
	HRESULT hr = MainWindow.CoCreateInstance(CLSID_MainWindow);
	if (SUCCEEDED(hr))
	{
		MainWindow->AddOperationStatus(strStatus.AllocSysString(), bPostMessage);
	}

}

HRESULT CAddinSimulationManager::OnStopSimulation(BSTR ExperimentName)
{
	SetSimulationStatus(FALSE);
	return S_OK;
}
HRESULT CAddinSimulationManager::OnInitializeLogFileInfo(BSTR ExperimentName)
{
	CComPtr<IExperiment> Experiment;
	HRESULT HR = Experiment.CoCreateInstance(CLSID_Experiment);
	if (SUCCEEDED(HR))
	{
		//Set Polygon Crystal Header File Info
		Experiment->WriteCSVLogFileHeaderInfo(CString("Angle,X,Y,Z\n").AllocSysString());
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}
void CAddinSimulationManager::OnInitializeSimulationGraph(BSTR ExperimentName)
{
	if (m_strExperimentType == CPP_SAMPLE_EXPERIMENT_TYPE_PLUS_TWO)
	{
		m_pPlusTwoPhysicsExperiment->InitializeSimulationGraph(ExperimentName);
	}
}
void CAddinSimulationManager::OnInitializeSimulationVideoRecording(BSTR ExperimentName)
{

}
void CAddinSimulationManager::OnNewDocument(BSTR DocumentName)
{

}
BOOL CAddinSimulationManager::OnDocumentOpened(BSTR DocumentPath)
{
	if (LoadDataFromDocument())
	{
		//Load the Default Selection
		LoadDefaultSelection();
		return S_OK;
	}
	else
	{
		return E_NOTIMPL;
	}
}

void CAddinSimulationManager::LoadDefaultSelection()
{
	CComPtr<IExperiment> Experiment;
	HRESULT hr = Experiment.CoCreateInstance(CLSID_Experiment);
	if (SUCCEEDED(hr))
	{
		BSTR SelectedExperiment;
		long SessionID;
		Experiment->GetSelectedExperiment(&SelectedExperiment, &SessionID);
		if (m_pAddin->m_lSessionID == SessionID)
		{
			LoadExperiments(m_strExperimentType);
			m_pAddin->SetRibbonControlText(CString(_T("Cpp.Sample.Experimental.Setup.Select.Experiment")), m_strExperimentType);
			if (m_strExperimentName != _T(""))
			{
				OnTreeNodeDblClick(SessionID, m_strRootText.AllocSysString(), m_strExperimentGroup.AllocSysString(), m_strExperimentName.AllocSysString());
				//Now selcet the tree now and Exand it
				CComPtr<IExperimentTreeView> ExperimentTreeView;
				HRESULT HR = ExperimentTreeView.CoCreateInstance(CLSID_ExperimentTreeView);
				if (FAILED(HR))
				{
					return;
				}
				ExperimentTreeView->SetTreeGroupState(m_strExperimentGroup.AllocSysString(), TVE_EXPAND);
				ExperimentTreeView->SelectActiveExperiment(SessionID, m_strExperimentGroup.AllocSysString(), m_strExperimentName.AllocSysString());
				OnReloadExperiment(SessionID, m_strRootText.AllocSysString(), m_strExperimentGroup.AllocSysString(), m_strExperimentName.AllocSysString());
			}
		}
	}
}
void CAddinSimulationManager::OnCloseDocument(BSTR DocumentPath)
{

}
void CAddinSimulationManager::OnBeforeSaveDocument(BSTR DocumentPath)
{
	SetDataToDocument();
}
void CAddinSimulationManager::OnDrawSimulation()
{
	
}
void CAddinSimulationManager::OnInitializeSimulation(long b3DMode, long VisualizationMode, BSTR Experiment)
{
	m_b3DMode = b3DMode;
	m_lVisualizationMode = VisualizationMode;
	LoadOtherSimulationOptions();
	//Reload the selected experiment to reflect the Visualization Mode
	OnReloadExperiment(m_pAddin->m_lSessionID, m_strRootText.AllocSysString(), m_strExperimentGroup.AllocSysString(), m_strExperimentName.AllocSysString());
	
}
void CAddinSimulationManager::OnDrawPredefinedScene(BSTR Experiment)
{

}
void CAddinSimulationManager::OnOwnerDrawSimulation()
{

}
void CAddinSimulationManager::OnOwnerDrawCreate()
{

}
void CAddinSimulationManager::ViewWndProc(long MsgID, VARIANT wParam, VARIANT lParam)
{

}
void CAddinSimulationManager::OnActivateView(long bActivate, BSTR CurrentViewFilePath, BSTR PreviousViewFilePath)
{
	if (bActivate)
	{
		LoadDefaultSelection();
	}
		
}
void CAddinSimulationManager::OnApplicationLaunched()
{

}
void CAddinSimulationManager::OnApplicationClose()
{

}

void CAddinSimulationManager::MianWndProc(long MsgID, VARIANT wParam, VARIANT lParam)
{

}
void CAddinSimulationManager::OnPropertyChanged(BSTR GroupName, BSTR PropertyName, BSTR PropertyValue)
{
	if (CString(GroupName) == OBJECT_PROPERTIES_TITLE)
	{
		m_pPlusTwoPhysicsExperiment->OnPropertyChanged(GroupName, PropertyName, PropertyValue);
	}
	
}
void CAddinSimulationManager::OnBeforeAddinControlsLoad()
{

}
void CAddinSimulationManager::OnAfterAddinControlsLoad() 
{

}
void CAddinSimulationManager::GetControlStatus(BSTR CtrlID, long * pStatus)
{
	*pStatus = TRUE;
}
void CAddinSimulationManager::RibbonWndProc(long MsgID, VARIANT wParam, VARIANT lParam)
{

}

void CAddinSimulationManager::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_strExperimentType;
		ar << m_strRootText;
		ar << m_strExperimentGroup;
		ar << m_strExperimentName;

		m_pPlusTwoPhysicsExperiment->Serialize(ar);
		m_pBScPhysicsExperiment->Serialize(ar);
		m_pMScPhysicsExperiment->Serialize(ar);

	}
	else
	{
		ar >> m_strExperimentType;
		ar >> m_strRootText;
		ar >> m_strExperimentGroup;
		ar >> m_strExperimentName;

		m_pPlusTwoPhysicsExperiment->Serialize(ar);
		m_pBScPhysicsExperiment->Serialize(ar);
		m_pMScPhysicsExperiment->Serialize(ar);
	}
}


BOOL CAddinSimulationManager::LoadDataFromDocument()
{
	//We will be loading all the data from the file as a Plugin Data File.
	//with a Key Value . The value will be set to base 64 format. 

	CComPtr<IApplicationDocument> ApplicationDocument;
	HRESULT HR = ApplicationDocument.CoCreateInstance(CLSID_ApplicationDocument);
	BSTR EncodedData = _T("");
	if (SUCCEEDED(HR))
	{
		ApplicationDocument->GetAddinSettingsAsString(m_strPluginName.AllocSysString(), CComBSTR(CPP_SAMPLE_DOC_SETTINGS_KEY), &EncodedData);
		CStringA strEncodedData(EncodedData);
		if (strEncodedData == _T(""))
		{
			return FALSE;
		}
		//AfxMessageBox(CString(strEncodedData));
		CBase64DecodeInfo Info;
		BOOL bResult = DecodeBase64(strEncodedData, Info);
		if (!bResult)
		{
			return FALSE;
		}
		//Load the data to data strructure
		CMemFile theFile(Info.m_pData, Info.m_Length);
		CArchive archive(&theFile, CArchive::load);
		Serialize(archive);
		archive.Close();

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


void CAddinSimulationManager::SetDataToDocument()
{
	CMemFile theFile;
	CArchive archive(&theFile, CArchive::store);
	Serialize(archive);
	archive.Close();

	int nSize = (int)theFile.GetLength();
	BYTE* mData = theFile.Detach();

	CStringA EncodedData = ToBase64(mData, nSize);
	//Save it Document Object

	CComPtr<IApplicationDocument> ApplicationDocument;
	HRESULT HR = ApplicationDocument.CoCreateInstance(CLSID_ApplicationDocument);
	if (SUCCEEDED(HR))
	{
		ApplicationDocument->SetAddinSettingsAsString(m_strPluginName.AllocSysString(), CComBSTR(CPP_SAMPLE_DOC_SETTINGS_KEY), EncodedData.AllocSysString());

	}
}

CStringA CAddinSimulationManager::ToBase64(const void* bytes, int byteLength)
{
	ASSERT(0 != bytes);

	CStringA base64;
	int base64Length = Base64EncodeGetRequiredLength(byteLength);

	VERIFY(Base64Encode(static_cast<const BYTE*>(bytes),
		byteLength,
		base64.GetBufferSetLength(base64Length),
		&base64Length));

	base64.ReleaseBufferSetLength(base64Length);
	return base64;
}

BOOL CAddinSimulationManager::DecodeBase64(CStringA Base64Data, CBase64DecodeInfo & Info)
{
	int iSrcLen = Base64Data.GetLength();
	int iRstLen = Base64DecodeGetRequiredLength(iSrcLen);
	iRstLen++;
	Info.m_Length = iRstLen;
	Info.m_pData = new BYTE[iRstLen];
	memset(Info.m_pData, 0, iRstLen);
	BOOL bRet = Base64Decode(Base64Data, iSrcLen, Info.m_pData, &Info.m_Length);
	return bRet;
}

BOOL CAddinSimulationManager::IsAddinExperimentSelected()
{
	//This routine should not be called form another thread 
	//It will fail
	
	CComPtr<IExperiment> Experiment;
	HRESULT hr = Experiment.CoCreateInstance(CLSID_Experiment);
	if (SUCCEEDED(hr))
	{
		m_strSelectedExperiment = _T("");
		static BSTR SelectedExperiment;
		long SessionID;
		Experiment->GetSelectedExperiment(&SelectedExperiment, &SessionID);
		if (m_pAddin->m_lSessionID != SessionID)
		{
			return FALSE;
		}
		m_strSelectedExperiment = SelectedExperiment;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CAddinSimulationManager::SetStatusBarMessage(CString strStatus, BOOL bPostMessage)
{
	_tcscpy(m_strCurrentStatusBarMessage, strStatus);

	CComPtr<IMainWindow> MainWindow;
	HRESULT hr = MainWindow.CoCreateInstance(CLSID_MainWindow);
	if (SUCCEEDED(hr))
	{
		MainWindow->SetStatusbarMessage(strStatus.AllocSysString(), bPostMessage);
	}

}

void CAddinSimulationManager::ResetPropertyGrid()
{
	CComPtr<IPropertyWindow> PropertyWindow;
	HRESULT HR = PropertyWindow.CoCreateInstance(CLSID_PropertyWindow);
	CString strGroupName = _T("");
	if (SUCCEEDED(HR))
	{
		PropertyWindow->RemoveAll();

		PropertyWindow->EnableHeaderCtrl(FALSE);
		PropertyWindow->EnableDescriptionArea(TRUE);
		PropertyWindow->SetVSDotNetLook(TRUE);
		PropertyWindow->MarkModifiedProperties(TRUE, TRUE);

	}
}

void CAddinSimulationManager::ResetAllStatusWindows()
{
	CComPtr<IMainWindow> MainWindow;
	HRESULT hr = MainWindow.CoCreateInstance(CLSID_MainWindow);
	if (SUCCEEDED(hr))
	{
		MainWindow->ResetAllStatusWindows();
	}
}

void CAddinSimulationManager::SetSimulationStatus(BOOL bActive)
{
	m_bSimulationActive = bActive;
}

void CAddinSimulationManager::LoadOtherSimulationOptions()
{
	CComPtr<IApplicationDocument> ApplicationDocument;
	HRESULT HR = ApplicationDocument.CoCreateInstance(CLSID_ApplicationDocument);
	if (SUCCEEDED(HR))
	{
		LONG Status;
		ApplicationDocument->get_LogToCSVFileStatus(&Status);
		m_bLogSimulationResultsToCSVFile = Status;

		ApplicationDocument->get_DisplayRealTimeGraphStatus(&Status);
		m_bDisplayRealTimeGraph = Status;

		ApplicationDocument->get_RecordSimulationAsVideoStatus(&Status);
		m_bRecordSimulationAsVideo = Status;

		ApplicationDocument->get_DisplayExpParamStatus(&Status);
		m_bShowExperimentalParamaters = Status;
		

	}
}

void CAddinSimulationManager::LogSimulationPoint(CString strLogData)
{
	//Log to file using Interfaces
	CComPtr<IExperiment> Experiment;
	HRESULT hr = Experiment.CoCreateInstance(CLSID_Experiment);
	if (SUCCEEDED(hr))
	{
		Experiment->WriteToCSVLogFile(strLogData.AllocSysString());
	}
}

