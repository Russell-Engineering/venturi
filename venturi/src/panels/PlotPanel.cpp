#include "PlotPanel.h"
#include "resources.h"

#include <thread>

namespace wi::Venturi
{
	static std::vector<std::thread> s_ThreadPool;
	static std::mutex mutex;
	PlotPanel::PlotPanel(Ref<DataManager> dataset)
		: Oak::Panel(), id(++m_idGenerator), m_DataManager(dataset)
	{
		//vk::Signals::SignalSpec spec = { };
		//AddSeries<SignalDataSeries>(SignalDataSeries("Trace 1", 10000, spec));

		//spec.amp = 1.5f;
		//spec.offset = 2.5f;
		//spec.waveform = vk::Signals::SQUARE;
		//AddSeries<SignalDataSeries>(SignalDataSeries("Trace 2", 10000, spec));

		//spec.amp = 0.5f;
		//spec.offset = -2.0f;
		//spec.waveform = vk::Signals::TRIANGLE;
		//AddSeries<SignalDataSeries>(SignalDataSeries("Trace 3", 10000, spec));	spec.amp = 0.5f;

		//spec.offset = 0.0f;
		//spec.waveform = vk::Signals::RANDOM;
		//AddSeries<SignalDataSeries>(SignalDataSeries("Trace 4", 10000, spec));
	}

	PlotPanel::~PlotPanel()
	{
		LOG_TRACE_TAG("~PlotPanel", "clearing plots ...");
		m_Plots.clear();
		//m_DataManager.clear();

	}

	// =========== EVENTS

	void PlotPanel::OnEvent(Oak::Event& e)
	{
		Oak::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<DataStreamEvent>(OAK_BIND_EVENT_FN(PlotPanel::OnStream));
		dispatcher.Dispatch<DataClearEvent>(OAK_BIND_EVENT_FN(PlotPanel::OnClear));
		dispatcher.Dispatch<DataSeriesCreateEvent>(OAK_BIND_EVENT_FN(PlotPanel::OnCreate));
		dispatcher.Dispatch<DataSeriesRemoveEvent>(OAK_BIND_EVENT_FN(PlotPanel::OnRemove));
	}
	
	bool PlotPanel::OnStream(DataStreamEvent& e)
	{
		return false;
	}
	bool PlotPanel::OnClear(DataClearEvent& e)
	{
		return false;
	}
	bool PlotPanel::OnCreate(DataSeriesCreateEvent& e)
	{
		for (auto& plot : m_Plots)
		{
			plot->RegisterSeries(e.GetSeriesSpec().ID, true); // the data series may not yet be created, but the plot will not try to display it if thats the case ... i think
		}
		return false; // not handled
	}

	bool PlotPanel::OnRemove(DataSeriesRemoveEvent& e)
	{
		return false;
	}



	// ============ UI
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

		ImGui::Begin(name, &open);	
		DrawToolBar();
		if (m_ShowPlotOptions)
		{	
			DrawPlotOptions();
			ImGui::SameLine();
		}

		ImGui::BeginChild("##PLOTWINDOW", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false, ImGuiWindowFlags_NoScrollbar);
		//ImGui::BeginChild("##PLOTWINDOW", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetWindowHeight() - ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_NoScrollbar);
				//float plot_height = (ImGui::GetWindowHeight() - ImGui::GetFrameHeightWithSpacing()) / (float)m_Plots.size();
		float plot_height = ImGui::GetContentRegionAvail().y / (float)m_Plots.size();
		for (auto& plot : m_Plots)
		{
			ImGui::BeginChild(std::string("##CHILD").append(plot->GetStrID()).c_str(), ImVec2(ImGui::GetContentRegionAvail().x, plot_height), false, ImGuiWindowFlags_NoScrollbar);
			plot->GetSettings().RangeX = ImVec2(0, m_History);
			plot->Draw();
			ImGui::EndChild();
		}
		ImGui::EndChild(); 
		Oak::UI::RenderMessageBoxes();
		ImGui::End();
	}




	void PlotPanel::DrawToolBar()
	{
		ImVec2 buttonSize = ImVec2(24, 24);
	

		ImGui::BeginChild("##PLOTTOOLBAR", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_NoScrollbar);

		ImGui::SetCursorPosY(ImGui::GetFrameHeightWithSpacing() / 2.0f - buttonSize.y / 2.0f);
		if (Oak::UI::ImageButton(Resources::SettingsIcon, buttonSize))
			m_ShowPlotOptions ^= true;

		ImGui::SameLine();
		if (Oak::UI::ImageButton(Resources::AddPlotIcon, buttonSize))
		{
			UI_ShowPlotCreationPopUp();
		}
		ImGui::SameLine();
		if (Oak::UI::ImageButton(Resources::PlusIcon, buttonSize))
		{
			UI_ShowDataCreationPopUp();
		}

		buttonSize = ImVec2(16, 16);
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0f - 2.0f * buttonSize.x);//todo fix this to use only the dockspace width
		ImGui::SetCursorPosY(ImGui::GetFrameHeightWithSpacing() / 2.0f - buttonSize.y / 2.0f);
		//if (m_clock->running && !m_clock->paused)
		//{
		//	if (Oak::UI::ImageButton(Resources::PauseIcon, buttonSize))
		//	{
		//		m_clock->pause();
		//	}
		//}
		//else 
		{
			if (Oak::UI::ImageButton(Resources::PlayIcon, buttonSize))
			{
				Oak::Application::Get().DispatchEvent<DataStreamEvent, true>(true);
			/*	if (!m_clock.IsRunning())
				{
					for (auto& [id, _series] : m_DataManager)
						_series->Erase();
				}

				m_clock.start();
				for (auto& [id, _series] : m_DataManager)
				{	
					wi::Ref<SignalDataSeries> series = _series.As<SignalDataSeries>();
					m_clock.tick(series->Signal.GetSpec().samplerate, series->buff, mutex);
				}
			*/
			}

		}
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetFrameHeightWithSpacing() / 2.0f - buttonSize.y / 2.0f);
		if (Oak::UI::ImageButton(Resources::StopIcon, buttonSize))
		{	
			Oak::Application::Get().DispatchEvent<DataStreamEvent, true>(false);
			//m_clock.stop();
		}
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetFrameHeightWithSpacing() / 2.0f - buttonSize.y / 2.0f);
		if (Oak::UI::ImageButton(Resources::RefreshIcon, buttonSize))
		{
			Oak::Application::Get().DispatchEvent<DataStreamEvent, true>(false);
			Oak::Application::Get().DispatchEvent<DataClearEvent, true>();
			Oak::Application::Get().DispatchEvent<DataStreamEvent, true>(true);
			//m_clock.stop();
			//for (auto& [id, _series] : m_DataManager)
			//{
			//	wi::Ref<SignalDataSeries> instance = _series.As<SignalDataSeries>();
			//	instance->Erase();
			//	//m_clock.tick(instance->Signal.GetSpec().samplerate, instance->Data);
			//}
			//m_clock.start();
			//for (auto& [id, _series] : m_DataManager)
			//{
			//	wi::Ref<SignalDataSeries> series = _series.As<SignalDataSeries>();
			//	m_clock.tick(series->Signal.GetSpec().samplerate, series->buff, mutex);
			//}
		}
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetFrameHeightWithSpacing() / 2.0f - buttonSize.y / 2.0f);
		if (true) //todo add recording to file
		{
			if (Oak::UI::ImageButton(Resources::RecordingIcon, buttonSize))
			{
				Oak::Application::Get().DispatchEvent<DataRecordEvent, true>(false);
			}
		}
		else
		{
			if (Oak::UI::ImageButton(Resources::RecordIcon, buttonSize))
			{
				Oak::Application::Get().DispatchEvent<DataStreamEvent, true>(true);
				Oak::Application::Get().DispatchEvent<DataRecordEvent, true>(true);
			}

		}
		ImGui::EndChild();
	}
	
	void PlotPanel::DrawPlots()
	{


	}

	void PlotPanel::DrawPlotOptions()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5, 5));
		ImGui::BeginChild("#OPTIONSTABBAR", ImVec2(ImGui::GetContentRegionMax().x * 0.15f, ImGui::GetContentRegionAvail().y), false, ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoScrollbar);
		
		if (ImGui::BeginTabBar("##SETTINGSTABBAR"))
		{
			if (ImGui::BeginTabItem("Plot Settings"))
			{
				ImGui::BeginChild("#PLOTSETTINGS", ImVec2(ImGui::GetContentRegionMax().x, ImGui::GetContentRegionAvail().y), false, ImGuiWindowFlags_AlwaysUseWindowPadding);

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
						Oak::UI::Property("Downsample", plot->GetSettings().Downsample, 1, 1000);
						Oak::UI::BeginCheckboxGroup("Data Series");
						std::string subheader = fmt::format("{0}##{1}", "Select Data", plot->GetID());
						if (Oak::UI::PropertyGridHeader(subheader, false))
						{
							for (auto& [id, series] : m_DataManager->GetDataSet())
							{
								if (Oak::UI::PropertyCheckboxColorGroup(series.Name.c_str(), plot->GetSettings().IncludeData[id], {(float)series.Color.x, (float)series.Color.y, (float)series.Color.z, (float)series.Color.w}))
								{
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

				ImGui::BeginChild("#SERIESSETTINGS", ImVec2(ImGui::GetContentRegionMax().x, ImGui::GetContentRegionAvail().y), false, ImGuiWindowFlags_AlwaysUseWindowPadding);
				int count = 0;
				for (auto& [id, series] : m_DataManager->GetDataSet())
				{
					std::string header = fmt::format("Data Series No. {0}##{1}", count, series.ID);
					if (Oak::UI::PropertyGridHeader(header, false))
					{
						Oak::UI::BeginPropertyGrid();
						
						Oak::UI::BeginDisabled();
						Oak::UI::Property("Series ID", series.ID);
						glm::uvec2 data_size = glm::uvec2(series.Data->RawData->x.size(), series.Data->RawData->y.size());
						Oak::UI::Property("Size", data_size);
						Oak::UI::EndDisabled();
						
						Oak::UI::Property("Series Name", series.Name, "Series Name", false);
						Oak::UI::Property("Buffer Size", series.BuffSize);
						Oak::UI::PropertyColor("Line Color", (glm::vec4&)series.Color);
						
						int selectedIOType =(int)GetIOType(series.IO);
						if (Oak::UI::PropertyDropdown("Series IO Type", SeriesIOStrings, (int)SeriesIO::_COUNT, &selectedIOType))
						{
							switch (selectedIOType)
							{
							case (int)SeriesIO::OSCIILLATOR:
							{
								series.IO = Ref<OscillatorIO>::Create(m_DataManager->GetClock(), vk::Signals::SignalSpec());
								break;
							}
							case (int)SeriesIO::DATASTREAM:
							{
								series.IO = Ref<DataStreamIO>::Create();
								break;
							}
							case (int)SeriesIO::FILESTREAM:
							{
								//series.IO = Ref<FileStreamIO>::Create();
								series.IO = Ref<UndefinedIO>::Create();
								break;
							}
							case (int)SeriesIO::UNDEFINED:
							{
								series.IO = Ref<UndefinedIO>::Create();
								break;
							}
							}
						}
						switch (selectedIOType)
						{
						case (int)SeriesIO::OSCIILLATOR:
						{
							
							Oak::UI::Property("Frequency (Hz)", series.IO.As<OscillatorIO>()->Signal.GetSpec().freq);
							Oak::UI::Property("Sample Rate", series.IO.As<OscillatorIO>()->Signal.GetSpec().samplerate);
							Oak::UI::Property("Amplitude", series.IO.As<OscillatorIO>()->Signal.GetSpec().amp);
							Oak::UI::Property("Offset", series.IO.As<OscillatorIO>()->Signal.GetSpec().offset);
							Oak::UI::Property("Phase", series.IO.As<OscillatorIO>()->Signal.GetSpec().phi);
							

							int selectedWaveform = (int)series.IO.As<OscillatorIO>()->Signal.GetSpec().waveform;
							if (Oak::UI::PropertyDropdown("Waveform", vk::Signals::WaveformStrings, vk::Signals::_COUNT, &selectedWaveform)) {}
								series.IO.As<OscillatorIO>()->Signal.SetWaveform((vk::Signals::Waveform)selectedWaveform);
							break;
						}
						case (int)SeriesIO::DATASTREAM:
						{
							break;
						}

						}

						Oak::UI::EndPropertyGrid();
						ImGui::TreePop();
					}
					count++;
				}
				
				//for (auto& [id, _series] : m_DataManager)
				//{
				//	auto series = _series.As<SignalDataSeries>();
				//	std::string header = fmt::format("Data Series {0}", series.Name);
				//	if (Oak::UI::PropertyGridHeader(header, false))
				//	{
				//		Oak::UI::PushID();
				//		Oak::UI::BeginPropertyGrid();
				//		Oak::UI::Property("Series Name", series->Name, "Series Name", false);
				//		Oak::UI::BeginDisabled();
				//		glm::uvec2 data_size = glm::uvec2(series->x.size(), series->y.size());
				//		Oak::UI::Property("Size", data_size);
				//		Oak::UI::EndDisabled();
				//		Oak::UI::Property("Max Size", series->Buffersize);
				//		Oak::UI::Property("Frequency (Hz)", series->Signal.GetSpec().freq);
				//		Oak::UI::Property("Sample Rate", series->Signal.GetSpec().samplerate);
				//		Oak::UI::Property("Amplitude", series->Signal.GetSpec().amp);
				//		Oak::UI::Property("Offset", series->Signal.GetSpec().offset);
				//		Oak::UI::Property("Phase", series->Signal.GetSpec().phi);
				//		Oak::UI::PropertyColor("Line Color", series->Color);

				//		int selectedWaveform = (int)series->Signal.GetSpec().waveform;
				//		if (Oak::UI::PropertyDropdown("Waveform", vk::Signals::WaveformStrings, vk::Signals::_COUNT, &selectedWaveform))
				//			series->Signal.SetWaveform((vk::Signals::Waveform)selectedWaveform);

				//		if (ImGui::Button("Default"))
				//			series->Signal.SetSpec(vk::Signals::SignalSpec());

				//		Oak::UI::EndPropertyGrid();
				//		Oak::UI::PopID();
				//		ImGui::TreePop();
				//	}
				//}

				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			
			if (ImGui::BeginTabItem("Info"))
			{
				for (auto& plot : m_Plots)
					ImGui::Text("Plot %d ('%s'): %d", plot->GetID(), plot->GetName().c_str(), plot->GetRefCount());

				//for (auto& [_id, _series] : m_DataManager)
				//	ImGui::Text("DataSet %d ('%s'): %d", _series->SeriesID, _series->Name.c_str(), _series->GetRefCount());

				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::EndChild();
		ImGui::PopStyleVar();
	}

	void PlotPanel::AddPlot(PlotSpec& _settings)
	{
		wi::Ref<Plot> plot = wi::Ref<Plot>::Create(_settings, m_DataManager);
		LOG_TRACE_TAG("PlotPanel::AddPlot", "Creating Plot {0} ('{1}') ...", plot->GetID(), plot->GetName());
		for (auto& plot : m_Plots)
		{
			for (const auto& [id, series] : m_DataManager->GetDataSet())
				plot->RegisterSeries(id, false);
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
		auto it = std::find_if(m_Plots.begin(), m_Plots.end(), [&](wi::Ref<Plot> plot) {return plot->GetID() == _uuid; });
		if (it != m_Plots.end())
			m_Plots.erase(it);
	}
	
	//template<typename T>
	//void PlotPanel::AddSeries(T& _dataSeries)
	//{
	//	static_assert(std::is_base_of<Oak::DataSeries<f64_t>, T>::value, "PlotPanel::AddSeries requires TSeries to inherit from Oak::DataSeries");

	//	LOG_TRACE_TAG("Plot::AddSeries", "Adding DataSeries {0} ('{1}')", _dataSeries.SeriesID, _dataSeries.Name.c_str());
	//	if (m_DataManager.find(_dataSeries.SeriesID) != m_DataManager.end())
	//	{
	//		LOG_WARN_TAG("Plot::AddSeries", "........DataSeries {0} ('{1}') already exsists!", _dataSeries.SeriesID, _dataSeries.Name.c_str());
	//		return;
	//	}
	//	auto series = wi::Ref<T>::Create(std::move(_dataSeries));
	//	m_DataManager.emplace(series->SeriesID, std::move(series));
	//}

	
	//void PlotPanel::RemoveSeries(Oak::UUID& _uuid)
	//{
	//	m_DataManager.erase(_uuid);
	//}

	void PlotPanel::UI_ShowDataCreationPopUp()
	{
		LOG_ERROR("TEST");
		Oak::UI::ShowMessageBox("Create Data Series", [&]()
			{
				vk::Signals::SignalSpec sigspec;
				sigspec.freq = 1.0f;
				sigspec.amp = 1.5f;
				sigspec.offset = 2.5f;
				sigspec.waveform = vk::Signals::SINE;
				sigspec.samplerate = 25600.0f;
				OscillatorIO io = { m_DataManager->GetClock(), sigspec };
				uint64_t id = wi::UUID();
				DataSeriesSpec spec = { id, "Name", Ref<DataSeries>::Create(), Ref<OscillatorIO>::Create(m_DataManager->GetClock(), sigspec), (size_t)100000, (size_t)25600};
				ImGui::Separator();
				if (ImGui::Button("OK") || ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeyPadEnter))
				{
					Oak::Application::Get().DispatchEvent<DataSeriesCreateEvent, true>(spec);
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel") || ImGui::IsKeyPressed(ImGuiKey_Escape))
					ImGui::CloseCurrentPopup();
			}, 400);
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
