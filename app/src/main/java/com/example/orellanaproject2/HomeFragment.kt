package com.example.orellanaproject2

import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import com.github.anastr.speedviewlib.SpeedView
import com.google.firebase.database.*

class HomeFragment : Fragment() {

    private lateinit var gaugeTemp: SpeedView
    private lateinit var gaugeUmid: SpeedView
    private lateinit var gaugeSolo: SpeedView

    private lateinit var database: DatabaseReference
    private lateinit var listener: ValueEventListener

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_home, container, false)

        gaugeTemp = view.findViewById(R.id.gaugeTemperatura)
        gaugeUmid = view.findViewById(R.id.gaugeUmidade)
        gaugeSolo = view.findViewById(R.id.gaugeSolo)

        setupGauges()
        fetchFirebaseData()

        return view
    }

    private fun setupGauges() {
        gaugeTemp.unit = "°C"
        gaugeUmid.unit = "%"
        gaugeSolo.unit = "%"

        listOf(gaugeTemp, gaugeUmid, gaugeSolo).forEach {
            it.minSpeed = 0f
            it.maxSpeed = 100f
            it.withTremble = false
        }
    }

    private fun fetchFirebaseData() {
        database = FirebaseDatabase.getInstance().getReference("Sensores")

        listener = object : ValueEventListener {
            override fun onDataChange(snapshot: DataSnapshot) {
                try {
                    val temp = snapshot.child("Temperatura").getValue(Float::class.java) ?: 0f
                    val umidade = snapshot.child("Umidade").getValue(Float::class.java) ?: 0f
                    val solo = snapshot.child("SensorDeUmidadeDoSolo").getValue(Float::class.java) ?: 0f

                    Log.d("Firebase", "Temp: $temp | Umid: $umidade | Solo: $solo")

                    gaugeTemp.speedTo(temp)
                    gaugeUmid.speedTo(umidade)
                    gaugeSolo.speedTo(solo)

                } catch (e: Exception) {
                    Log.e("HomeFragment", "Erro ao processar dados do Firebase", e)
                }
            }

            override fun onCancelled(error: DatabaseError) {
                Log.e("Firebase", "Erro ao ler dados", error.toException())
            }
        }

        database.addValueEventListener(listener)
    }

    override fun onDestroyView() {
        super.onDestroyView()
        if (::database.isInitialized && ::listener.isInitialized) {
            database.removeEventListener(listener)
        }
    }
}
