#pragma once

class Zone
{
public:
	Zone() = delete;
	~Zone() = default;
	explicit Zone(const ZoneId inZoneId);

private:
	const ZoneId zoneId{};
};