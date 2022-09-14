#include "PlotPanel.h"
#include "resources.h"

namespace Venturi
{
	PlotPanel::PlotPanel()
		: Oak::Panel(), id(++m_idGenerator)
	{
		m_clock = Oak::CreateScope<vk::Clock>();
		m_SigGen = Oak::CreateScope<vk::Signals::SignalGenerator>((vk::Signals::Waveform)m_selectedWaveform);
		
		AddSeries(DataSeries<vk::Vec2>(Oak::UUID(), "Trace 1", 25000));
		AddSeries(DataSeries<vk::Vec2>(Oak::UUID(), "Trace 2", 25000));
		AddSeries(DataSeries<vk::Vec2>(Oak::UUID(), "Trace 3", 25000));
	}

	PlotPanel::~PlotPanel()
	{
		OAK_TRACE_TAG("~PlotPanel", "clearing plots ...");
		m_Plots.clear();
		OAK_TRACE_TAG("~PlotPanel", "clearing Datasets ...");
		m_DataSet.clear();

	}
	void PlotPanel::PushLocalStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImPlotStyle& plot_style = ImPlot::GetStyle();
		plot_style.Colors[ImPlotCol_FrameBg] = ImVec4(0, 0, 0, 0);
		plot_style.Colors[ImPlotCol_PlotBorder] = ImVec4(1, 1, 1, 0.25);
		plot_style.PlotPadding = ImVec2(5, 0);

		ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_WindowBg]); m_ColorPopCount++;
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_WindowBg]); m_ColorPopCount++;
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_WindowBg]); m_ColorPopCount++;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5,0)); m_StylePopCount++;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5,0)); m_StylePopCount++;

	}

	void PlotPanel::CreatDataSeries()
	{
		AddSeries(DataSeries<vk::Vec2>());
	}
	
	void PlotPanel::OnUIRender(const char* name, bool& open)
	{
		ImGui::Begin(name, &open);
		DrawToolBar();
		if (m_ShowPlotOptions)
		{	
			DrawPlotOptions();
			ImGui::SameLine();
		}

		ImGui::BeginChild("##PLOTWINDOW", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight()), ImGuiWindowFlags_NoScrollbar);
		DrawPlots();
		ImGui::EndChild(); 
		ImGui::End();
	}
	void PlotPanel::AddSeries(DataSeries<vk::Vec2>& _dataSeries)
	{
		OAK_TRACE_TAG("Plot::AddSeries", "Adding DataSeries {0} ('{1}')", _dataSeries.SeriesID, _dataSeries.Name.c_str());
		if (m_DataSet.find(_dataSeries.SeriesID) != m_DataSet.end())
		{
			OAK_WARN_TAG("Plot::AddSeries", "........DataSeries {0} ('{1}') already exsists!", _dataSeries.SeriesID, _dataSeries.Name.c_str());
			return;
		}
		auto series = Oak::Ref<DataSeries<vk::Vec2>>::Create(std::move(_dataSeries));
		m_DataSet.emplace(series->SeriesID, std::move(series));
	}


	void PlotPanel::AddPlot()
	{
		struct PlotSettings settings = {};
		settings.PlotID = Oak::UUID();
		settings.Type = PlotType::REALTIME;
		Oak::Ref<Plot> plot =  Oak::Ref<Plot>::Create(settings);
		OAK_TRACE_TAG("PlotPanel::AddPlot", "Creating Plot {0} ('{1}') ...", plot->GetID(), plot->GetName());

		for (auto& [_id, _series] : m_DataSet)
		{
			plot->AddSeries(_series);
		}
		m_Plots.push_back(plot);
	}

	void PlotPanel::UpdateData()
	{

	}

	void PlotPanel::DrawPlots()
	{
		vk::f64_t t, y;
		if (m_Running)
		{
			t = m_clock->poll();
			y = m_SigGen->Generate(t);
			for (auto& [_id, _series] : m_DataSet)
			{
				_series->AddPoint(vk::Vec2(t, y));
			}
		}	

		float plot_height = (ImGui::GetWindowHeight() - ImGui::GetFrameHeightWithSpacing()) / (float)m_Plots.size();

		for (auto& plot : m_Plots)
		{
			ImGui::BeginChild(std::string("##CHILD").append(plot->GetStrID()).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, plot_height), ImGuiWindowFlags_NoScrollbar);
			plot->GetSettings().RangeX = ImVec2(t - m_History, t);
			plot->Draw();
			ImGui::EndChild();
		}
	}

	void PlotPanel::DrawToolBar()
	{
		ImVec2 buttonSize = ImVec2(24, 24);
	

		ImGui::BeginChild("##PLOTTOOLBAR", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeightWithSpacing()), ImGuiWindowFlags_NoScrollbar);

		ImGui::SetCursorPosY(ImGui::GetFrameHeightWithSpacing() / 2.0f - buttonSize.y / 2.0f);
		if (Oak::ImageButton(Resources::SettingsIcon, buttonSize))
			m_ShowPlotOptions ^= true;

		ImGui::SameLine();
		if (Oak::ImageButton(Resources::PlusIcon, buttonSize))
		{
			AddPlot();
		}

		buttonSize = ImVec2(16, 16);
		ImGui::SameLine(ImGui::GetWindowContentRegionMax().x / 2.0f - 1.5f * buttonSize.x);
		ImGui::SetCursorPosY(ImGui::GetFrameHeightWithSpacing() / 2.0f - buttonSize.y / 2.0f);
		if (m_Running)
		{
			if (Oak::ImageButton(Resources::PauseIcon, buttonSize))
			{
				m_clock->pause();
				m_Running = false;
			}
		}
		else
		{
			if (Oak::ImageButton(Resources::PlayIcon, buttonSize))
			{
				m_clock->resume();
				m_Running = true;
			}

		}
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetFrameHeightWithSpacing() / 2.0f - buttonSize.y / 2.0f);
		if (Oak::ImageButton(Resources::StopIcon, buttonSize))
		{
			m_clock->stop();
			m_Running = false;
		}
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetFrameHeightWithSpacing() / 2.0f - buttonSize.y / 2.0f);
		if (m_Recording)
		{
			if (Oak::ImageButton(Resources::RecordingIcon, buttonSize))
			{
				m_Recording = false;
			}
		}
		else
		{
			if (Oak::ImageButton(Resources::RecordIcon, buttonSize))
			{
				m_Recording = true;
			}

		}
		ImGui::EndChild();
	}
	
	void PlotPanel::DrawPlotOptions()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));
		ImGui::BeginChild("#PLOTOPTIONS", ImVec2(ImGui::GetContentRegionMax().x * 0.25f, ImGui::GetContentRegionAvail().y), ImGuiWindowFlags_AlwaysUseWindowPadding);
		ImGui::InputFloat("Frequency", &m_freq, 1, 10.0 * M_PI, "%.1f PI (rad)");
		m_SigGen->SetFrequency(m_freq * M_PI);

		ImGui::InputFloat("History", &m_History, 1, 300, "%.1f s");

		ImGui::Combo("Waveform", &m_selectedWaveform, m_waveforms, IM_ARRAYSIZE(m_waveforms));
		m_SigGen->SetWaveForm((vk::Signals::Waveform)m_selectedWaveform);

		

		for (auto& plot : m_Plots)
		{
			ImGui::Separator();
			ImGui::InputTextWithHint(std::string("##TITLE").append(plot->GetStrID()).c_str(), "Title", &plot->GetSettings().Title);
			ImGui::InputTextWithHint(std::string("##XLABEL").append(plot->GetStrID()).c_str(), "x-label", &plot->GetSettings().LabelX);
			ImGui::InputTextWithHint(std::string("##YLABEL").append(plot->GetStrID()).c_str(), "y-label", &plot->GetSettings().LabelY);

		}

		if (ImGui::Button("remove last plot"))
			m_Plots.pop_back();

		ImGui::Separator();
		ImGui::Separator();
		for (auto& plot : m_Plots)
		{
			ImGui::Text("Plot %d ('%s'): %d", plot->GetID(), plot->GetName().c_str(), plot->GetRefCount());

		}

		ImGui::Separator();
		Oak::UUID to_remove;
		for (auto& [_id, _series] : m_DataSet)
		{
			//ImGui::Text("DataSet %d ('%s'): %d", _series.SeriesID, _series.Name.c_str(), _series.GetRefCount());
			ImGui::Text("DataSet %d ('%s'): %d", _series->SeriesID, _series->Name.c_str(), _series->GetRefCount());
		}

	

		ImGui::EndChild();
		ImGui::PopStyleVar();
	}

	void PlotPanel::RemovePlot(Oak::UUID uuid)
	{

	}
}