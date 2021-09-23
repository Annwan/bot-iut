const parseICSDate = str => {
    let time={
		year: +str.substr(0, 4),
		month: +str.substr(4, 2)-1,
		day: +str.substr(6, 2),
		hours: 0,
		minutes: 0,
		seconds: 0
	}
	if(str.substr(8, 1)=='T') {
		time.hours=+str.substr(9, 2);
		time.minutes=+str.substr(11, 2);
		time.seconds=+str.substr(13, 2);
	}
	let date;
	if(str.endsWith('Z')) {
		date=new Date(Date.UTC(time.year, time.month, time.day, time.hours, time.minutes, time.seconds, 0));
	} else {
		date=new Date(time.year, time.month, time.day, time.hours, time.minutes, time.seconds, 0);
	}
	return date;
}


const dateFormater = new Intl.DateTimeFormat("fr-FR", {
    timeZone: 'Europe/Paris',
    hour12: false,
    hour: '2-digit',
    minute: '2-digit',
    second: '2-digit',
    day: '2-digit',
    month: '2-digit',
    year: 'numeric'
});

const dateToFields = date => {
    let parts = dateFormater.formatToParts(date);
    let keys = {};
    for (var i = 0; i < parts.length; i++) {
        keys[ parts[i].type ] = parts[i].value
    }
    keys.weekday = date.getDay();
    return keys;
}


class Event {
    constructor() {
        this.start = null;
        this.end = null;
        this.name = '[]';
        this.desc = '[]';
        this.loc = '';
    }

    get stringRepr() {
        let s = dateToFields(this.start)
        let e = dateToFields(this.end)
        let t = this.desc.match(/^\\n\\n.+\\n(.+)\\n/)
        t = t ? t[1] : 'Unknown'
        return `${s.hour}:${s.minute} -> ${e.hour}:${e.minute}: **${this.name}** en *${this.loc}* avec *${t}*`
    }

    get date() {
        let s = dateToFields(this.start)
        return `${s.day}/${s.month}/${s.year}`
    }

}

const parseICS = ics => {
    let lines = ics.split('\r\n')
    var inEvent = fals
    var curent = null
    let events = []
    for (let line of lines){
        let {key, value} = line.split(':')
        if (line === 'BEGIN:VEVENT' && !inEvent) {
            current = new Event()
            inEvent = true
            continue
        }
        if (!inEvent) continue
        if (line === 'END:VEVENT') {
            events.push(curent);
            inEvent = false
            continue
        }
        if (key === 'SUMMARY') {
            current.name = value
            continue
        }
        if (key === 'DESCRIPTION') {
            current.desc = value
            continue
        }
        if (key === 'DTSTART') {
            current.start = parseICSDate(value)
            continue
        }
        if (key === 'DTEND') {
            current.end = parseICSDate(value)
            continue
        }
        if (key === 'LOCATION') {
            current.loc = value
        }
    }

    events.sort((a, b) => a.start - b.start)
}

const eventsBetweenDates = (es, ds, de) => {
    let ces = []
    for (let e of es) {
        if (e.start >= ds && e.end <= de) ces.push(e) 
    }
    return ces
}

exports = (url, ds, de) => {
    let req = await fetch(url)
    let ics = await req.text()
    return eventsBetweenDates(parseICS(ics), ds, de)
}