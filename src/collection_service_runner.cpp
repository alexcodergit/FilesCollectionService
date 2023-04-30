#include "collection_service_runner.hpp"
#include "scan_service.hpp"
#include "copy_service.hpp"

void fcs::CollectionServiceRunner::StartService(std::ostream& os)
{
	if (m_service_running) {
		os << __FUNCTION__ << ": Data Collection Service already running." << std::endl;
		return;
	}
	if (m_file_name_pattern.empty()) {
		os << __FUNCTION__ << ": File Name Pattern not specified." << std::endl;
		return;
	}
	if (m_scan_directories.empty()) {
		os << __FUNCTION__ << ": Source/Target directories not specified." << std::endl;
		return;
	}

	m_message_buffer.m_messages.clear();

	fcs::ScanService scan_service(m_file_name_pattern, m_message_buffer, m_scan_directories);
	fcs::CopyService copy_service(m_message_buffer);

	m_j_scan_thread = std::jthread(&fcs::ScanService::ScanDirectories, scan_service, m_stop_source.get_token());
	m_j_copy_thread = std::jthread(&fcs::CopyService::RunCopyService, copy_service, m_stop_source.get_token());

	m_service_running = true;
	os << __FUNCTION__ << ": Data Collection Service is running..." << std::endl;
}

void fcs::CollectionServiceRunner::StopService(std::ostream& os)
{
	if (!m_service_running) {
		os << __FUNCTION__ << ": Data Collection Service is not running..." << std::endl;
		return;
	}
	m_stop_source.request_stop();
	std::this_thread::sleep_for(std::chrono::milliseconds(2));
	if (m_j_scan_thread.joinable()) {
		m_j_scan_thread.join();
	}
	if (m_j_copy_thread.joinable()) {
		m_j_copy_thread.join();
	}
	os << __FUNCTION__ << ": Data Collection Service stopped..." << std::endl;
	m_service_running = false;
}

bool fcs::CollectionServiceRunner::AssignFileNamePattern(const std::string fname_pattern, std::ostream& os)
{
	if (m_service_running) {
		os << __FUNCTION__ << ": Data Collection Service is running..." << std::endl;
		return false;
	}
	if (fname_pattern.empty()) {
		os << __FUNCTION__ << ": File name pattern not specified." << std::endl;
		return false;
	}
	m_file_name_pattern = fname_pattern;
	return true;
}

bool fcs::CollectionServiceRunner::AddDirectoryMapping(const std::string source, const std::string target, std::ostream& os)
{
	if (m_service_running) {
		os << __FUNCTION__ << ": Data Collection Service is running..." << std::endl;
		return false;
	}
	if (source.empty()) {
		os << __FUNCTION__ << ": Source directory not specified." << std::endl;
		return false;
	}
	if (target.empty()) {
		os << __FUNCTION__ << ": Target directory not specified." << std::endl;
		return false;
	}
	std::filesystem::path spath(source);
	if (!std::filesystem::exists(spath) || !std::filesystem::is_directory(spath)) {
		os << __FUNCTION__ << ": Source directory is not valid directory." << std::endl;
		return false;
	}
	spath = std::filesystem::path(target);
	if (!std::filesystem::exists(spath) || !std::filesystem::is_directory(spath)) {
		os << __FUNCTION__ << ": Target directory is not valid directory." << std::endl;
		return false;
	}
	std::filesystem::copy_options options = std::filesystem::copy_options::update_existing |
		std::filesystem::copy_options::recursive;

	m_scan_directories.push_back({
		std::filesystem::path(source) ,
		std::filesystem::path(""),
		std::filesystem::path(target),
		options });

	return true;
}

fcs::CollectionServiceRunner::~CollectionServiceRunner()
{
	if (m_service_running) {
		StopService();
	}
}