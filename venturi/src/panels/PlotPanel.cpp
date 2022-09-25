#include "PlotPanel.h"
#include "resources.h"

#include <thread>

namespace Venturi
{
	static std::vector<std::thread> s_ThreadPool;
	static std::mutex mutex;
	PlotPanel::PlotPanel()
		: Oak::Panel(), id(++m_idGenerator)
	{

		vk::Signals::SignalSpec spec = { };
		AddSeries<SignalDataSeries>(SignalDataSeries("Trace 1", 10000, spec));

		spec.amp = 1.5f;
		spec.offset = 2.5f;
		spec.waveform = vk::Signals::SQUARE;
		AddSeries<SignalDataSeries>(SignalDataSeries("Trace 2", 10000, spec));

		spec.amp = 0.5f;
		spec.offset = -2.0f;
		spec.waveform = vk::Signals::TRIANGLE;
		AddSeries<SignalDataSeries>(SignalDataSeries("Trace 3", 10000, spec));	spec.amp = 0.5f;

		spec.offset = 0.0f;
		spec.waveform = vk::Signals::RANDOM;
		AddSeries<SignalDataSeries>(SignalDataSeries("Trace 4", 10000, spec));
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
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 0)); m_StylePopCount++;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 0)); m_StylePopCount++;
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,0.0f); m_StylePopCount++;

	}

	
	void PlotPanel::OnUIRender(const char* name, bool& open)
	{
		for (auto& [id, _series] : m_DataSet)
		{
			Oak::Ref<SignalDataSeries> series = _series.As<SignalDataSeries>();
			mutex.lock();
			series->OnUpdate();
			mutex.unlock();
		}

		ImGui::Begin(name, &open);
		DrawToolBar();
		if (m_ShowPlotOptions)
		{	
			DrawPlotOptions();
			ImGui::SameLine();
		}

		ImGui::BeginChild("##PLOTWINDOW", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight() - ImGui::GetFrameHeightWithSpacing()), ImGuiWindowFlags_NoScrollbar);
		DrawPlots();
		ImGui::EndChild(); 
		Oak::UI::RenderMessageBoxes();
		ImGui::End();
	}
	
	void PlotPanel::DrawToolBar()
	{
		ImVec2 buttonSize = ImVec2(24, 24);
	

		ImGui::BeginChild("##PLOTTOOLBAR", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeightWithSpacing()), ImGuiWindowFlags_NoScrollbar);

		ImGui::SetCursorPosY(ImGui::GetFrameHeightWithSpacing() / 2.0f - buttonSize.y / 2.0f);
		if (Oak::UI::ImageButton(Resources::SettingsIcon, buttonSize))
			m_ShowPlotOptions ^= true;

		ImGui::SameLine();
		if (Oak::UI::ImageButton(Resources::PlusIcon, buttonSize))
		{
			UI_ShowPlotCreationPopUp();
		}

		buttonSize = ImVec2(16, 16);
		ImGui::SameLine(ImGui::GetWindowContentRegionMax().x / 2.0f -2.0f * buttonSize.x);
		ImGui::SetCursorPosY(ImGui::GetFrameHeightWithSpacing() / 2.0f - buttonSize.y / 2.0f);
		if (m_clock.IsRunning() && !m_clock.IsPaused())
		{
			if (Oak::UI::ImageButton(Resources::PauseIcon, buttonSize))
			{
				m_clock.pause();
			}
		}
		else 
		{
			if (Oak::UI::ImageButton(Resources::PlayIcon, buttonSize))
			{
				if (!m_clock.IsRunning())
				{
					for (auto& [id, _series] : m_DataSet)
						_series->Erase();
				}

				m_clock.start();
				for (auto& [id, _series] : m_DataSet)
				{	
					Oak::Ref<SignalDataSeries> series = _series.As<SignalDataSeries>();
					m_clock.tick(series->Signal.GetSpec().samplerate, series->buff, mutex);
				}
			}

		}
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetFrameHeightWithSpacing() / 2.0f - buttonSize.y / 2.0f);
		if (Oak::UI::ImageButton(Resources::StopIcon, buttonSize))
		{	
			m_clock.stop();
		}
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetFrameHeightWithSpacing() / 2.0f - buttonSize.y / 2.0f);
		if (Oak::UI::ImageButton(Resources::RefreshIcon, buttonSize))
		{
			m_clock.stop();
			for (auto& [id, _series] : m_DataSet)
			{
				Oak::Ref<SignalDataSeries> instance = _series.As<SignalDataSeries>();
				instance->Erase();
				//m_clock.tick(instance->Signal.GetSpec().samplerate, instance->Data);
			}
			m_clock.start();
			for (auto& [id, _series] : m_DataSet)
			{
				Oak::Ref<SignalDataSeries> series = _series.As<SignalDataSeries>();
				m_clock.tick(series->Signal.GetSpec().samplerate, series->buff, mutex);
			}
		}
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetFrameHeightWithSpacing() / 2.0f - buttonSize.y / 2.0f);
		if (true) //todo add recording to file
		{
			if (Oak::UI::ImageButton(Resources::RecordingIcon, buttonSize))
			{
			}
		}
		else
		{
			if (Oak::UI::ImageButton(Resources::RecordIcon, buttonSize))
			{
			}

		}
		ImGui::EndChild();
	}
	
	void PlotPanel::DrawPlots()
	{

		float plot_height = (ImGui::GetWindowHeight() - ImGui::GetFrameHeightWithSpacing()) / (float)m_Plots.size();
		for (auto& plot : m_Plots)
		{
			ImGui::BeginChild(std::string("##CHILD").append(plot->GetStrID()).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, plot_height), ImGuiWindowFlags_NoScrollbar);
			plot->GetSettings().RangeX = ImVec2(0,m_History);
			plot->Draw();
			ImGui::EndChild();
		}
	}

	void PlotPanel::DrawPlotOptions()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));
		ImGui::BeginChild("#OPTIONSTABBAR", ImVec2(ImGui::GetContentRegionMax().x * 0.15f, ImGui::GetContentRegionAvail().y), ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
		
		if (ImGui::BeginTabBar("##SETTINGSTABBAR"))
		{
			if (ImGui::BeginTabItem("Plot Settings"))
			{
				ImGui::BeginChild("#PLOTSETTINGS", ImVec2(ImGui::GetContentRegionMax().x, ImGui::GetContentRegionAvail().y), ImGuiWindowFlags_AlwaysUseWindowPadding);

				for (auto& plot : m_Plots)
				{
					std::string header = fmt::format("{0}##{1}", plot->GetSettings().Name, plot->GetID());
					if (Oak::UI::PropertyGridHeader(header, false))
					{
						Oak::UI::PushID();
						Oak::UI::BeginPropertyGrid();
						Oak::UI::Property("Title", plot->GetSettings().Title, "Title", false);
						Oak::UI::Property("x-axis label", plot->GetSettings().LabelX, "x-label", false);
						Oak::UI::Property("y-axis label", plot->GetSettings().LabelY, "y-label", false);
						Oak::UI::BeginCheckboxGroup("Data Series");
						std::string subheader = fmt::format("{0}##{1}", "Select Data", plot->GetID());
						if (Oak::UI::PropertyGridHeader(subheader, false))
						{
							for (auto& [id, series] : m_DataSet)
							{
								if (Oak::UI::PropertyCheckboxColorGroup(series->Name.c_str(), plot->GetSettings().IncludeData[id], series->Color))
								{
									if (plot->GetSettings().IncludeData[id]) plot->AddSeries(series);
									else plot->RemoveSeries(series);
								}
							}
							ImGui::TreePop();
						}
						Oak::UI::EndCheckboxGroup();
						//ImGui::NextColumn();
						int selectedPlotTye = (int)plot->GetSettings().Type;
						if (Oak::UI::PropertyDropdown("Plot Type", PlotTypeStrings, PlotType::_COUNT, &selectedPlotTye))
							plot->GetSettings().Type = (PlotType)selectedPlotTye;



						Oak::UI::PopID();
						Oak::UI::EndPropertyGrid();
						ImGui::TreePop();
					}
				}
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Series Settings"))
			{

				ImGui::BeginChild("#SERIESSETTINGS", ImVec2(ImGui::GetContentRegionMax().x, ImGui::GetContentRegionAvail().y), ImGuiWindowFlags_AlwaysUseWindowPadding);
				for (auto& [id, _series] : m_DataSet)
				{
					auto series = _series.As<SignalDataSeries>();
					std::string header = fmt::format("Data Series {0}", series->SeriesID);
					if (Oak::UI::PropertyGridHeader(header, false))
					{
						Oak::UI::PushID();
						Oak::UI::BeginPropertyGrid();
						Oak::UI::Property("Series Name", series->Name, "Series Name", false);
						Oak::UI::BeginDisabled();
						glm::uvec2 data_size = glm::uvec2(series->x.size(), series->y.size());
						Oak::UI::Property("Size", data_size);
						Oak::UI::EndDisabled();
						Oak::UI::Property("Max Size", series->Buffersize);
						Oak::UI::Property("Frequency (Hz)", series->Signal.GetSpec().freq);
						Oak::UI::Property("Sample Rate", series->Signal.GetSpec().samplerate);
						Oak::UI::Property("Amplitude", series->Signal.GetSpec().amp);
						Oak::UI::Property("Offset", series->Signal.GetSpec().offset);
						Oak::UI::Property("Phase", series->Signal.GetSpec().phi);
						Oak::UI::PropertyColor("Line Color", series->Color);

						int selectedWaveform = (int)series->Signal.GetSpec().waveform;
						if (Oak::UI::PropertyDropdown("Waveform", vk::Signals::WaveformStrings, vk::Signals::_COUNT, &selectedWaveform))
							series->Signal.SetWaveform((vk::Signals::Waveform)selectedWaveform);

						if (ImGui::Button("Default"))
							series->Signal.SetSpec(vk::Signals::SignalSpec());

						Oak::UI::EndPropertyGrid();
						Oak::UI::PopID();
						ImGui::TreePop();
					}
				}

				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			
			if (ImGui::BeginTabItem("Info"))
			{
				for (auto& plot : m_Plots)
					ImGui::Text("Plot %d ('%s'): %d", plot->GetID(), plot->GetName().c_str(), plot->GetRefCount());

				for (auto& [_id, _series] : m_DataSet)
					ImGui::Text("DataSet %d ('%s'): %d", _series->SeriesID, _series->Name.c_str(), _series->GetRefCount());

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::EndChild();
		ImGui::PopStyleVar();
	}

	void PlotPanel::AddPlot(PlotSpec& _settings)
	{

		Oak::Ref<Plot> plot = Oak::Ref<Plot>::Create(_settings);
		OAK_TRACE_TAG("PlotPanel::AddPlot", "Creating Plot {0} ('{1}') ...", plot->GetID(), plot->GetName());

		for (auto& [_id, _series] : m_DataSet)
		{
			plot->GetSettings().IncludeData[_id] = false;
		}
		m_Plots.push_back(plot);
	}
	
	void PlotPanel::CreatePlot()
	{

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowBgAlpha(0.8f);

	}

	void PlotPanel::RemovePlot(Oak::UUID& _uuid)
	{
		auto it = std::find_if(m_Plots.begin(), m_Plots.end(), [&](Oak::Ref<Plot> plot) {return plot->GetID() == _uuid; });
		if (it != m_Plots.end())
			m_Plots.erase(it);
	}
	
	template<typename T>
	void PlotPanel::AddSeries(T& _dataSeries)
	{
		static_assert(std::is_base_of<Oak::DataSeries<vk::f64_t>, T>::value, "PlotPanel::AddSeries requires TSeries to inherit from Oak::DataSeries");

		OAK_TRACE_TAG("Plot::AddSeries", "Adding DataSeries {0} ('{1}')", _dataSeries.SeriesID, _dataSeries.Name.c_str());
		if (m_DataSet.find(_dataSeries.SeriesID) != m_DataSet.end())
		{
			OAK_WARN_TAG("Plot::AddSeries", "........DataSeries {0} ('{1}') already exsists!", _dataSeries.SeriesID, _dataSeries.Name.c_str());
			return;
		}
		auto series = Oak::Ref<T>::Create(std::move(_dataSeries));
		m_DataSet.emplace(series->SeriesID, std::move(series));
	}

	void PlotPanel::CreateSeries()
	{
		// popup to select data series type
		AddSeries<Oak::DataSeries<vk::f64_t>>(Oak::DataSeries<vk::f64_t>());
	}
	
	void PlotPanel::RemoveSeries(Oak::UUID& _uuid)
	{
		m_DataSet.erase(_uuid);
	}
	
	void PlotPanel::UI_ShowPlotCreationPopUp()
	{
		AddPlot(PlotSpec(Oak::UUID()));
		
		Oak::UI::ShowMessageBox("Create Plot", [&]()
			{
				auto& plot = m_Plots.back();
			//PlotSettings settings = PlotSettings(Oak::UUID());
				// this doesnt live ... maybe i need to make this a reference to settings, then implement the copy/move constructors for other cases where we create plot settings
				ImGui::Text("Define Plot Properties");
				ImGui::Separator();
				ImGui::InputTextWithHint("##TITLE", "Enter plot title.. ", &plot->GetSettings().Title);
				ImGui::InputTextWithHint("##XLABEL", "x-label.. ", &plot->GetSettings().LabelX);
				ImGui::InputTextWithHint("##YLABEL", "y-label.. ", & plot->GetSettings().LabelY);
				if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeyPadEnter))
					ImGui::CloseCurrentPopup();
				ImGui::SameLine();
				if (ImGui::Button("Cancel") || ImGui::IsKeyPressed(ImGuiKey_Escape))
				{
					m_Plots.pop_back();
					ImGui::CloseCurrentPopup();
				}
			}, 400);
	}


	
}