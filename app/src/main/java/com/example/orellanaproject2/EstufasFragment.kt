package com.example.orellanaproject2

import GraficoFragment
import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button

class EstufasFragment : Fragment() {

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        return inflater.inflate(R.layout.fragment_estufas, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        val btnGrafico1 = view.findViewById<Button>(R.id.btnGrafico1)
        btnGrafico1.setOnClickListener {
            GraficoFragment("Temperatura").show(parentFragmentManager, "grafico")
        }
        val btnGrafico2 = view.findViewById<Button>(R.id.btnGrafico2)
        btnGrafico2.setOnClickListener {
            GraficoFragment("Umidade").show(parentFragmentManager, "grafico")
        }
        val btnGrafico3 = view.findViewById<Button>(R.id.btnGrafico3)
        btnGrafico3.setOnClickListener {
            GraficoFragment("Umidade do Solo").show(parentFragmentManager, "grafico")
        }
        val btnGrafico4 = view.findViewById<Button>(R.id.btnGrafico4)
        btnGrafico4.setOnClickListener {
            GraficoFragment("Luminosidade").show(parentFragmentManager, "grafico")
        }
    }
}
