def lire_logs(filename):
    logs = []
    with open(filename, "r") as file:
        for line in file:
            date, heure, ip, port, proto, statut = line.strip().split(";")
            logs.append({
                "date": date,
                "heure": heure,
                "ip": ip,
                "port": int(port),
                "protocole": proto,
                "statut": statut
            })
    return logs


def analyser_logs(logs):

    total = len(logs)
    succes = sum(1 for l in logs if l["statut"] == "SUCCES")
    echec = total - succes

    ports = {}
    ips = {}

    for l in logs:
        ports[l["port"]] = ports.get(l["port"], 0) + 1
        ips[l["ip"]] = ips.get(l["ip"], 0) + 1

    max_port = max(ports, key=ports.get)
    max_ip = max(ips, key=ips.get)

    print("\n===== RESUME DES STATISTIQUES =====")
    print("Total connexions :", total)
    print("Succes :", succes)
    print("Echecs :", echec)
    print("Port le plus utilise :", max_port)
    print("IP la plus active :", max_ip)

    detecter_suspects(logs, ports, ips, total, succes, echec)


def detecter_suspects(logs, ports, ips, total, succes, echec):

    suspects = {}

    for l in logs:
        if l["statut"] == "ECHEC":
            key = (l["ip"], l["port"])
            suspects[key] = suspects.get(key, 0) + 1

    print("\n===== LA LISTE DES IPs SUSPECTES =====")

    with open("rapport_analyse.txt", "w") as f:

        f.write("===== RAPPORT D'ANALYSE RESEAU =====\n\n")
        f.write("Statistiques generales :\n")
        f.write(f"Total connexions : {total}\n")
        f.write(f"Succes : {succes}\n")
        f.write(f"Echecs : {echec}\n")
        f.write(f"Port le plus utilise : {max(ports, key=ports.get)}\n")
        f.write(f"IP la plus active : {max(ips, key=ips.get)}\n\n")

        f.write("Activites suspectes :\n")

        for (ip, port), count in suspects.items():
            if count > 5:
                print(f"IP suspecte : {ip} (Port {port})")
                f.write(f"{ip} (Port {port})\n")

        f.write("\nTop 3 des ports les plus utilises :\n")
        top_ports = sorted(ports.items(), key=lambda x: x[1], reverse=True)[:3]
        for i, (port, count) in enumerate(top_ports, 1):
            f.write(f"{i}. Port {port} : {count} connexions\n")


if __name__ == "__main__":
    logs = lire_logs("network_log.txt")
    analyser_logs(logs)
